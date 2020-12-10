#pragma once

#include <stdint.h>
#include <time.h>

typedef struct timespec Time;

Time GetCurrentTime(); // Get the current time

double GetTimeDifference(Time *now, Time *last); // Get the difference between `now` and `last` in `seconds.milliseconds` format

