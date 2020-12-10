#pragma once

#include <stdint.h>
#include <stddef.h>

void InitRNG(); // Initialize random number generator

uint64_t RNG64(); // Get a 64 bit random number

uint32_t RNG32(); // Get a 32 bit random number

uint16_t RNG16(); // Get a 32 bit random number

uint8_t RNG8(); // Get a 32 bit random number

uint8_t RNGBool(); // Get a random boolean

void FillRNG(uint8_t *ptr, size_t size); // Fill ptr with random numbers
