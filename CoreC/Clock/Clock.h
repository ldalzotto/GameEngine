#pragma once

#include <stdlib.h>

typedef struct Core_Clock
{
	size_t FrameCount;
	float DeltaTime;
} Core_Clock;

void Clock_newFrame(Core_Clock* p_clock);
void Clock_newUpdate(Core_Clock* p_clock, float p_delta);


typedef unsigned long long int TimeClockPrecision;

TimeClockPrecision core_currentTime_mics();