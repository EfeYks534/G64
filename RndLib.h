#pragma once

#include <stdint.h>
#include <stddef.h>

#define G64_PRIORITY_MEGA   0x0
#define G64_PRIORITY_ULTRA  0x1
#define G64_PRIORITY_VHIGH  0x2
#define G64_PRIORITY_HIGH   0x3
#define G64_PRIORITY_MEDIUM 0x4
#define G64_PRIORITY_SMALL  0x5
#define G64_PRIORITY_LOW    0x6
#define G64_PRIORITY_VLOW   0x7

void SetClockPriority(uint8_t priority);

int StartClock(); // Start high speed clock

uint64_t RNG64(); // Get a 64 bit random number

uint32_t RNG32(); // Get a 32 bit random number

uint16_t RNG16(); // Get a 32 bit random number

uint8_t RNG8(); // Get a 32 bit random number

uint8_t RNGBool(); // Get a random boolean

void FillRNG(uint8_t *ptr, size_t size); // Fill ptr with random numbers
