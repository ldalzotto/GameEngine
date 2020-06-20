#pragma once

namespace _GameEngine::_Clock
{
	struct Clock
	{
		size_t FrameCount;
		float DeltaTime;
	};

	Clock* Clock_alloc();
	void Clock_free(Clock** p_clock);

	void Clock_newFrame(Clock* p_clock);
	void Clock_newUpdate(Clock* p_clock, float p_delta);
}