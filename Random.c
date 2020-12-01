#include "RndLib.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>
#include <pthread.h>

static uint8_t    _clock[16];
static uint8_t       started;
static uint8_t      priority;
static uint8_t   manual_mode;
static pthread_t      thread;

static void *TickClock(void *_arg)
{
	uint64_t steps = 0;
	while(1) {
		for(int i = 0; i < 16; i++) {
			steps++;
			for(int j = 0; j < 16; j++) {
				_clock[i] ++;
				_clock[i] += (++_clock[j]) ^ (_clock[i]);
				_clock[i] = (_clock[i] << (_clock[j] % 4)) |
						(_clock[i] >> (8 - (_clock[j] % 4)));
				if(priority > 0 && steps % 16 == 0) {
					manual_mode = 1;
					nanosleep(&(struct timespec){0, 256*priority}, NULL);
					manual_mode = 0;
				}
			}
		}
	}

	return NULL;
}

void SetClockPriority(uint8_t pri)
{
	switch(pri)
	{
	case G64_PRIORITY_MEGA: priority = 2; break;
	case G64_PRIORITY_ULTRA: priority = 4; break;
	case G64_PRIORITY_VHIGH: priority = 8; break;
	case G64_PRIORITY_HIGH: priority = 16; break;
	case G64_PRIORITY_MEDIUM: priority = 32; break;
	case G64_PRIORITY_SMALL: priority = 64; break;
	case G64_PRIORITY_LOW: priority = 128; break;
	case G64_PRIORITY_VLOW: priority = 255; break;
	default: priority = 16; break;
	}
}

int StartClock()
{
	if(started) return -1;
	SetClockPriority(G64_PRIORITY_MEDIUM);

	int res1 = pthread_create(&thread, NULL, TickClock, NULL);
	if(res1 != -1) started++;
	return (res1 == -1) ? 0 : 1;
}

static uint64_t seed;

static uint64_t ManualRNG()
{
	if(manual_mode) return 0;
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	seed = (time.tv_sec+1) * time.tv_nsec;
	return (seed << (time.tv_nsec % 36)) | (seed >> (time.tv_nsec % 24));
}

uint64_t RNG64()
{
	return (*(uint64_t*)_clock) ^ (*(uint64_t*)(&_clock[8])) ^ ManualRNG();
}

uint32_t RNG32()
{
	return (*(uint32_t*)(&_clock[4])) ^ (*(uint32_t*)(&_clock[0]))
				^ (*(uint32_t*)(&_clock[8])) ^ ManualRNG();
}

uint16_t RNG16()
{
	return (*(uint16_t*)(&_clock[6])) ^ (*(uint16_t*)(&_clock[2])) ^ ManualRNG();
}

uint8_t RNG8()
{
	return _clock[0] ^ _clock[3] ^ ManualRNG();
}

static uint8_t parity_table[] = {1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1};

uint8_t RNGBool()
{
	return parity_table[_clock[7]^(uint8_t)ManualRNG()];
}

void FillRNG(uint8_t *_ptr, size_t size)
{
	for(size_t i = 0; i < size; i++)
		_ptr[i] = _clock[i % 16] ^ _clock[i % 8] ^ ManualRNG();
}
