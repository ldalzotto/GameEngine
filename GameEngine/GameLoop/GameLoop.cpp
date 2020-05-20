#include "GameLoop.h"

#include "chrono"

namespace _GameEngine
{
	namespace _GameLoop
	{

		TimeClockPrecision get_currentTime_mics()
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		};

		GameLoop* alloc(TimeClockPrecision p_timeBetweenUpdates_mics)
		{
			GameLoop* l_instance = new GameLoop();
			l_instance->TimeBetweenUpdates_mics = p_timeBetweenUpdates_mics;
			l_instance->AccumulatedElapsedTime_mics = 0;
			l_instance->PreviousUpdateTime_mics = get_currentTime_mics();
			return l_instance;
		};

		void free(GameLoop** p_gameLoop)
		{
			delete *p_gameLoop;
			*p_gameLoop = nullptr;
		};

		void set_updateCallback(GameLoop* p_gameLoop, UpdateCallback p_updateCallback, void* p_closure)
		{
			p_gameLoop->UpdateCallback = p_updateCallback;
			p_gameLoop->UpdateCallbackClosure = p_closure;
		};

		void set_renderCallback(GameLoop* p_gameLoop, RenderCallback p_renderCallback, void* p_closure)
		{
			p_gameLoop->RenderCallback = p_renderCallback;
			p_gameLoop->RenderCallbackClosure = p_closure;
		};

		void update(GameLoop* p_gameLoop)
		{
			TimeClockPrecision l_currentTime = get_currentTime_mics();
			TimeClockPrecision l_elapsed = l_currentTime - p_gameLoop->PreviousUpdateTime_mics;

			p_gameLoop->PreviousUpdateTime_mics = l_currentTime;
			p_gameLoop->AccumulatedElapsedTime_mics += l_elapsed;

			// Input events

			if (p_gameLoop->AccumulatedElapsedTime_mics >= p_gameLoop->TimeBetweenUpdates_mics) {

				while (p_gameLoop->AccumulatedElapsedTime_mics >= p_gameLoop->TimeBetweenUpdates_mics)
				{
					p_gameLoop->UpdateCallback(p_gameLoop->UpdateCallbackClosure, p_gameLoop->TimeBetweenUpdates_mics * 0.000001f);
					p_gameLoop->AccumulatedElapsedTime_mics -= p_gameLoop->TimeBetweenUpdates_mics;
				}

				p_gameLoop->RenderCallback(p_gameLoop->RenderCallbackClosure);

			}
		};
	} // namespace _GameLoop

} // namespace _GameEngine
