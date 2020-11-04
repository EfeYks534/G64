#pragma once

#include <stdint.h>
#include <time.h>

struct timespec GetCurrentTime(); // Get the current time

double GetTimeDifference(struct timespec *now, struct timespec *last); // Get the difference between `now` and `last` in `seconds.milliseconds` format

