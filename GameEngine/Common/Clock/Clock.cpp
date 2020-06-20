#include "Clock.h"

namespace _GameEngine::_Clock
{
	Clock* Clock_alloc()
	{
		return new Clock();
	};

	void Clock_free(Clock** p_clock)
	{
		Clock* l_clock = *p_clock;
		delete l_clock;
		l_clock = nullptr;
		p_clock = nullptr;
	};

	void Clock_newFrame(Clock* p_clock)
	{
		p_clock->FrameCount += 1;
	};

	void Clock_newUpdate(Clock* p_clock, float p_delta)
	{
		p_clock->DeltaTime = p_delta;
	};
}