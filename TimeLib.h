#pragma once

#include <stdint.h>

void SetTimeDiff(uint8_t i); //  Mark the current time on `i`

double GetTimeDiff(uint8_t i); // Get the difference between current time and last marked time in `seconds.milliseconds` format

