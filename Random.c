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

void FillRNG(void *ptr, size_t size, uint32_t nmemb)
{
	uint8_t buf[size];

	for(int i = 0; i < nmemb; i++) {
		if(getrandom(&buf, size, 0) == -1)
			for(int j = 0; i < size; j++)
				buf[j] = (uint8_t) rand();
		memcpy((uint8_t*)ptr + size * i, buf, size);
	}
}
