#include "RndLib.h"
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>

uint64_t RNG()
{
	uint64_t val = 0;
	if(getrandom(&val, sizeof(val), 0) == -1) {
		srand(time(NULL));
		uint8_t *vals = (uint8_t*)&val;
		for(int i = 0; i < 8; i++)
			vals[i] = (uint8_t)rand();
	}
	return val;
}
