#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

extern EventGroupHandle_t wifiEventGroup; // Event group to manage WiFi connection status

void InitWifi(void);
void StreamToServer(void *pvParameters);
