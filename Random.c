#include "RndLib.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>

uint64_t RNG()
{
	uint64_t val = 0;
	if(getrandom(&val, sizeof(val), 0) == -1) {
		uint8_t *vals = (uint8_t*)&val;
		for(int i = 0; i < 8; i++)
			vals[i] = (uint8_t) rand();
	}
	return val;
}

void FillRNG(void *ptr, size_t size)
{
	if(getrandom(ptr, size, 0) == -1)
		for(int i = 0; i < size; i++)
			((uint8_t*)ptr)[i] = (uint8_t) rand();
}
