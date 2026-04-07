// Copyright (c) 2026 James Collins
// Licensed freely under the MIT License. See LICENSE in the project root for details.

#pragma once

#include <stdatomic.h>

#define FILENAME_TABLE_SIZE 256     // circular buffer capacity

extern atomic_int writeIndex;                           // Shared between WriteToSD and StreamToServer — atomic to prevent data race on concurrent read/write
extern char filenameTable[FILENAME_TABLE_SIZE][64];     // circular buffer of generated file paths, indexed by writeIndex / uploadIndex

void InitSD(void);
void QueueExistingFiles(void);
void TryRefillQueue(int uploadIndex);
void WriteToSD(void *pvParameters);
