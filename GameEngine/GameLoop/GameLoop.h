#pragma once

namespace _GameEngine
{
	typedef void (*NewFrameCallback)(void* p_closure);
	typedef void (*UpdateCallback)(void* p_closure, float p_delta);
	typedef void (*RenderCallback)(void* p_closure);
	typedef void (*EndOfFrameCallback)(void* p_closure);
	typedef unsigned long long int TimeClockPrecision;

	/**
		@brief Calling callbacks in a fixed order at a fixed time interval.

		The @ref GameLoop calls the @ref UpdateCallback and @ref RenderCallback at a fixed time interval defined
		by @ref TimeBetweenUpdates_mics.
			* The @ref UpdateCallback can be called multiple time if the system cannot keep up the rate.
			* The @ref RenderCallback will always be called once.
	*/
	struct GameLoop
	{
		int TimeBetweenUpdates_mics;
		TimeClockPrecision PreviousUpdateTime_mics;

		/**
			Defines the lag of the system clock compared to the previous frame.
		*/
		TimeClockPrecision AccumulatedElapsedTime_mics;

		NewFrameCallback NewFrameCallback;
		void* NewFrameCallbackClosure;

		UpdateCallback UpdateCallback;
		void* UpdateCallbackClosure;

		RenderCallback RenderCallback;
		void* RenderCallbackClosure;

		EndOfFrameCallback EndofFrameCallback;
		void* EndOfFrameClosure;
	};

	namespace _GameLoop
	{
		GameLoop* alloc(TimeClockPrecision p_timeBetweenUpdates_mics);
		void free(GameLoop** p_gameLoop);

		void set_newFrameCallback(GameLoop* p_gameLoop, NewFrameCallback p_newFrameCallback, void* p_closure);
		void set_updateCallback(GameLoop* p_gameLoop, UpdateCallback p_updateCallback, void* p_closure);
		void set_renderCallback(GameLoop* p_gameLoop, RenderCallback p_renderCallback, void* p_closure);
		void set_endOfFrameCallback(GameLoop* p_gameLoop, EndOfFrameCallback p_endOfFrameCallback, void* p_closure);

		/**
			@brief Execute callbacks for the fixed time rate.

			This function represents a single frame execution in the GameEngine.
		*/
		void update(GameLoop* p_gameLoop);
	} // namespace _GameLoop

} // namespace _GameEngine
