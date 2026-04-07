// Copyright (c) 2026 James Collins
// Licensed freely under the MIT License. See LICENSE in the project root for details.

#include <stdio.h>
#include <time.h>
#include <stdatomic.h>
#include "esp_sntp.h"
#include "nvs.h"
#include "config.h"
#include "clock.h"

#define NVS_NAMESPACE "clock"
#define NVS_KEY_TIME  "last_time"

// Set true by sntp_sync_callback() once a valid timestamp is received from the NTP server
static atomic_bool timeSynced = false;

// Load the last saved Unix timestamp from NVS and set the system clock
static void LoadSavedTime(void) {
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle) != ESP_OK) return;

    int64_t savedTime = 0;
    if (nvs_get_i64(handle, NVS_KEY_TIME, &savedTime) == ESP_OK && savedTime > 0) {
        struct timeval tv = { .tv_sec = (time_t)savedTime, .tv_usec = 0 };
        settimeofday(&tv, NULL);
        atomic_store(&timeSynced, true);
        printf("Clock: restored saved time from NVS\n");
    }
    nvs_close(handle);
}

// Persist the current Unix timestamp to NVS so it survives power cycles.
static void SaveCurrentTime(void) {
    nvs_handle_t handle;
    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle) != ESP_OK) return;

    time_t now;
    time(&now);
    nvs_set_i64(handle, NVS_KEY_TIME, (int64_t)now);
    nvs_commit(handle);
    nvs_close(handle);
}

// Called when a valid timestamp is received from the NTP server
static void sntp_sync_callback(struct timeval *tv) {
    atomic_store(&timeSynced, true);
    SaveCurrentTime(); // Persist the synced time so it survives power cycles
    printf("SNTP: time synchronised\n");
}

// Initialise SNTP and configure the timezone. Syncing happens in the background whenever WiFi is connected.
void InitSNTP(void) {
    setenv("TZ", TIMEZONE, 1); // Configure the local timezone
    tzset();
    LoadSavedTime(); // Restore last known time from NVS before SNTP syncs
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, NTP_SERVER);
    sntp_set_time_sync_notification_cb(sntp_sync_callback);
    esp_sntp_init();
}

// Returns true if SNTP has received a valid timestamp at least once since boot
bool IsTimeSynced(void) {
    return atomic_load(&timeSynced);
}

// Fills buffer with the current local time formatted as "YYYYMMDD_HHMMSS".
// Falls back to "00000000_000000" if the clock has not yet been synced via NTP.
void GetTimestamp(char *buffer, size_t length) {
    if (!atomic_load(&timeSynced)) {
        snprintf(buffer, length, "00000000_000000");
        return;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(buffer, length, "%Y%m%d_%H%M%S", &timeinfo);
}
