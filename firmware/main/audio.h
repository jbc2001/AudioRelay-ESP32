// Copyright (c) 2026 James Collins
// Licensed freely under the MIT License. See LICENSE in the project root for details.

#pragma once

#include <stdatomic.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s_std.h"

extern i2s_chan_handle_t rxChannel;         // I2S channel handle for audio input
extern QueueHandle_t audioQueue;            // Queue to hold audio data buffers
extern TaskHandle_t audioTaskHandle;        // Handle for the ReadAudioInput task (used by start/stop buttons)
extern atomic_bool recordingActive;         // Set false to pause recording, true to resume — atomic to prevent data race on concurrent read/write
extern atomic_bool clippingActive;          // Set true by ReadAudioInput when a clipped sample is detected — atomic to prevent data race on concurrent read/write

bool ConfigDMA(void);
void ReadAudioInput(void *pvParameters);
void ClippingIndicatorTask(void *pvParameters);
