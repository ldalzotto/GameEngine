#include "Clock.h"


void Clock_NewFrame(Clock_PTR p_clock)
{
	p_clock->FrameCount += 1;
};

void Clock_NewUpdate(Clock_PTR p_clock, float p_delta)
{
	p_clock->DeltaTime = p_delta;
};

#ifdef _WINDOWS

#include "Include/PlatformInclude.h"
#include <sysinfoapi.h>

TimeClockPrecision Clock_currentTime_mics()
{
	FILETIME l_currentTime;
	GetSystemTimeAsFileTime(&l_currentTime);
	ULARGE_INTEGER ul;
	ul.LowPart = l_currentTime.dwLowDateTime;
	ul.HighPart = l_currentTime.dwHighDateTime;
	TimeClockPrecision l_time = ul.QuadPart;
	l_time /= 10;
	return l_time;
};

#endif

