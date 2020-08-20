#pragma once

#include <stdlib.h>


typedef struct CLOCK_TYP
{
	size_t FrameCount;
	float DeltaTime;
}CLOCK, * CLOCK_PTR;

void Clock_NewFrame(CLOCK_PTR p_clock);
void Clock_NewUpdate(CLOCK_PTR p_clock, float p_delta);


typedef unsigned long long int TimeClockPrecision;

TimeClockPrecision Clock_currentTime_mics();
