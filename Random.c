#include "RndLib.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>
#include <pthread.h>

static pthread_mutex_t mutex;

static uint64_t table[4096];
static uint64_t         top;
static uint8_t        first;

void InitRNG()
{
	if(!first) {
		pthread_mutex_init(&mutex, NULL);
		first = 1;
	}
	uint64_t seed = 0;
	if(getrandom(&seed, sizeof(uint64_t), GRND_NONBLOCK) == -1) seed = 1;
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	srand(seed * (time(NULL)+1) * (t.tv_nsec + 1));
	for(int i = 0; i < 4096; i++)
		table[i] = ( ((uint64_t)rand() << 31) || rand() ) ^ ((uint64_t)rand() << 32);
	top = 0;
}

uint64_t RNG64()
{
	pthread_mutex_lock(&mutex);
	if(top >= 4096) InitRNG();
	uint64_t n = table[top++];
	pthread_mutex_unlock(&mutex);
	return n;
}

uint32_t RNG32()
{
	pthread_mutex_lock(&mutex);
	if(top >= 4096) InitRNG();
	uint64_t n = table[top++];
	pthread_mutex_unlock(&mutex);
	return n;
}

uint16_t RNG16()
{
	pthread_mutex_lock(&mutex);
	if(top >= 4096) InitRNG();
	uint64_t n = table[top++];
	pthread_mutex_unlock(&mutex);
	return n;
}

uint8_t RNG8()
{
	pthread_mutex_lock(&mutex);
	if(top >= 4096) InitRNG();
	uint64_t n = table[top++];
	pthread_mutex_unlock(&mutex);
	return n;
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
	pthread_mutex_lock(&mutex);
	if(top >= 4096) InitRNG();
	uint64_t n = table[top++];
	pthread_mutex_unlock(&mutex);
	return parity_table[n % 256];
}

void FillRNG(uint8_t *_ptr, size_t size)
{
	pthread_mutex_lock(&mutex);

	InitRNG();
	for(int i = 0; i < size; i++)
		_ptr[i] = rand();

	pthread_mutex_unlock(&mutex);
}
