#pragma once

#include <stdlib.h>


typedef struct Clock_TYP
{
	size_t FrameCount;
	float DeltaTime;
}Clock, * Clock_PTR;

void Clock_NewFrame(Clock_PTR p_clock);
void Clock_NewUpdate(Clock_PTR p_clock, float p_delta);


typedef unsigned long long int TimeClockPrecision;

TimeClockPrecision Clock_currentTime_mics();
