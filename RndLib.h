#pragma once

#include <stdint.h>
#include <stddef.h>

uint64_t RNG(); // Get a random number

void FillRNG(void *ptr, size_t size, uint32_t nmemb); // Fill ptr with random numbers
