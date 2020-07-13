#pragma once

#include <stdlib.h>

namespace _Core
{
	struct Clock
	{
		size_t FrameCount;
		float DeltaTime;
	};

	void Clock_newFrame(Clock* p_clock);
	void Clock_newUpdate(Clock* p_clock, float p_delta);


	typedef unsigned long long int TimeClockPrecision;

	TimeClockPrecision Clock_currentTime_mics();
}

