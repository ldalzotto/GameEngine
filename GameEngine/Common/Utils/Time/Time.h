#pragma once

#include "chrono"

namespace _GameEngine::_Utils
{

    typedef unsigned long long int TimeClockPrecision;

	TimeClockPrecision get_currentTime_mics()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	};

	TimeClockPrecision get_currentTime_ms()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	};
};