#define _GNU_SOURCE
#include "TimeLib.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static struct timespec *starts = NULL;
static struct timespec *ends = NULL;

void SetTimeDiff(uint8_t i)
{
	if(starts == NULL) starts = malloc(sizeof(struct timespec)*256);
	if(ends == NULL) ends = malloc(sizeof(struct timespec)*256);
	clock_gettime(CLOCK_MONOTONIC_RAW, &(starts[i]));
}

double GetTimeDiff(uint8_t i)
{
	if(starts == NULL) starts = malloc(sizeof(struct timespec)*256);
	if(ends == NULL) ends = malloc(sizeof(struct timespec)*256);
	clock_gettime(CLOCK_MONOTONIC_RAW, &(ends[i]));
	double seconds = ends[i].tv_sec-starts[i].tv_sec;
	double nanos = (ends[i].tv_nsec-starts[i].tv_nsec);
	return seconds+nanos/1000000000;
}

