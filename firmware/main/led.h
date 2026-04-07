// Copyright (c) 2026 James Collins
// Licensed freely under the MIT License. See LICENSE in the project root for details.

#pragma once

#include "led_strip.h"

extern led_strip_handle_t led; // LED strip handle for status indication

void InitLED(void);
void SetLED(uint32_t red, uint32_t green, uint32_t blue); // Sets the LED colour with brightness scaling applied per LED_BRIGHTNESS_SHIFT
