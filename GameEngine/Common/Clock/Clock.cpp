#include "Clock.h"

namespace _GameEngine::_Clock
{

	void Clock_newFrame(Clock* p_clock)
	{
		p_clock->FrameCount += 1;
	};

	void Clock_newUpdate(Clock* p_clock, float p_delta)
	{
		p_clock->DeltaTime = p_delta;
	};
}