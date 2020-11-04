#define _GNU_SOURCE
#include "TimeLib.h"
#include <stdint.h>
#include <time.h>

struct timespec GetCurrentTime()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	return t;
}

double GetTimeDifference(struct timespec *now, struct timespec *last)
{
	double secs = now->tv_sec - last->tv_sec;
	double nanos = now->tv_nsec - last->tv_nsec;
	return secs + nanos / 1000000000;
}

