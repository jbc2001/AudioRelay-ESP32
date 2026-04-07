// Copyright (c) 2026 James Collins
// Licensed freely under the MIT License. See LICENSE in the project root for details.

#pragma once

#include <stdatomic.h>
#include <stddef.h>

void InitSNTP(void);
bool IsTimeSynced(void);
void GetTimestamp(char *buffer, size_t length); // Fills buffer with "YYYYMMDD_HHMMSS", or "00000000_000000" if clock not yet synced via NTP
