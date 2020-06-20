#include "GameLoop.h"

#include "Utils/Time/Time.h"

namespace _GameEngine
{
	namespace _GameLoop
	{
		// To avoid crazy amount of update if the elapsed time from last frame is huge
		int MAX_UPDATE_CALL_PER_FRAME = 2;

		GameLoop* alloc(TimeClockPrecision p_timeBetweenUpdates_mics)
		{
			GameLoop* l_instance = new GameLoop();
			l_instance->TimeBetweenUpdates_mics = static_cast<int>(p_timeBetweenUpdates_mics);
			l_instance->AccumulatedElapsedTime_mics = 0;
			l_instance->PreviousUpdateTime_mics = _Utils::get_currentTime_mics();
			return l_instance;
		};

		void free(GameLoop** p_gameLoop)
		{
			delete* p_gameLoop;
			*p_gameLoop = nullptr;
		};

		void set_newFrameCallback(GameLoop* p_gameLoop, NewFrameCallback p_newFrameCallback, void* p_closure)
		{
			p_gameLoop->NewFrameCallback = p_newFrameCallback;
			p_gameLoop->NewFrameCallbackClosure = p_closure;
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

		void set_endOfFrameCallback(GameLoop* p_gameLoop, EndOfFrameCallback p_endOfFrameCallback, void* p_closure)
		{
			p_gameLoop->EndofFrameCallback = p_endOfFrameCallback;
			p_gameLoop->EndOfFrameClosure = p_closure;
		};

		void update(GameLoop* p_gameLoop)
		{
			TimeClockPrecision l_currentTime = _Utils::get_currentTime_mics();
			TimeClockPrecision l_elapsed = l_currentTime - p_gameLoop->PreviousUpdateTime_mics;

			if (l_elapsed > static_cast<TimeClockPrecision>(p_gameLoop->TimeBetweenUpdates_mics) * MAX_UPDATE_CALL_PER_FRAME)
			{
				l_elapsed = static_cast<TimeClockPrecision>(p_gameLoop->TimeBetweenUpdates_mics) * MAX_UPDATE_CALL_PER_FRAME;
			}

			p_gameLoop->PreviousUpdateTime_mics = l_currentTime;
			p_gameLoop->AccumulatedElapsedTime_mics += l_elapsed;

			if (p_gameLoop->AccumulatedElapsedTime_mics >= p_gameLoop->TimeBetweenUpdates_mics) {

				p_gameLoop->NewFrameCallback(p_gameLoop->NewFrameCallbackClosure);

				while (p_gameLoop->AccumulatedElapsedTime_mics >= p_gameLoop->TimeBetweenUpdates_mics)
				{
					p_gameLoop->UpdateCallback(p_gameLoop->UpdateCallbackClosure, p_gameLoop->TimeBetweenUpdates_mics * 0.000001f);
					p_gameLoop->AccumulatedElapsedTime_mics -= p_gameLoop->TimeBetweenUpdates_mics;
				}

				p_gameLoop->RenderCallback(p_gameLoop->RenderCallbackClosure);

			}

			p_gameLoop->EndofFrameCallback(p_gameLoop->EndOfFrameClosure);
		};
	} // namespace _GameLoop

} // namespace _GameEngine
