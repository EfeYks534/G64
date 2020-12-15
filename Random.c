#include "RndLib.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>
#include <pthread.h>

void InitRNG()
{
	uint64_t seed = 0;
	if(getrandom(&seed, sizeof(uint64_t), GRND_NONBLOCK) == -1) seed = 1;
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	srand(seed * (time(NULL)+1) * (t.tv_nsec + 1));
}

uint64_t RNG64()
{
	return (rand() << 31) | rand();
}

uint32_t RNG32()
{
	return rand();
}

uint16_t RNG16()
{
	return rand();
}

uint8_t RNG8()
{
	return rand();
}

static uint8_t parity_table[] = {
1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,
0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,
1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,
0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,
0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,
0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,
0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,
0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,
1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1};

uint8_t RNGBool()
{
	return parity_table[rand() & 0xFF];
}

void FillRNG(uint8_t *_ptr, size_t size)
{
	for(int i = 0; i < size; i++)
		_ptr[i] = rand();
}
