#include "GameLoop.h"

#include "Clock/Clock.h"


// To avoid crazy amount of update if the elapsed time from last frame is huge
int MAX_UPDATE_CALL_PER_FRAME = 2;

void GameLoop_build(GameLoop* p_gameLoop, TimeClockPrecision p_timeBetweenUpdates_mics)
{
	p_gameLoop->TimeBetweenUpdates_mics = (int)p_timeBetweenUpdates_mics;
	p_gameLoop->AccumulatedElapsedTime_mics = 0;
	p_gameLoop->PreviousUpdateTime_mics = Clock_currentTime_mics();
};

void GameLoop_free(GameLoop* p_gameLoop)
{
};

void GameLoop_SetnewFrameCallback(GameLoop* p_gameLoop, NewFrameCallback p_newFrameCallback, void* p_closure)
{
	p_gameLoop->NewFrameCallback = p_newFrameCallback;
	p_gameLoop->NewFrameCallbackClosure = p_closure;
};

void GameLoop_SetupdateCallback(GameLoop* p_gameLoop, UpdateCallback p_updateCallback, void* p_closure)
{
	p_gameLoop->UpdateCallback = p_updateCallback;
	p_gameLoop->UpdateCallbackClosure = p_closure;
};

void GameLoop_SetendOfUpdateCallback(GameLoop* p_gameLoop, EndOfFrameCallback p_endOfUpdateCallback, void* p_closure)
{
	p_gameLoop->EndOfUpdateCallback = p_endOfUpdateCallback;
	p_gameLoop->EndOfUpdateCallbackClosure = p_closure;
};

void GameLoop_SetrenderCallback(GameLoop* p_gameLoop, RenderCallback p_renderCallback, void* p_closure)
{
	p_gameLoop->RenderCallback = p_renderCallback;
	p_gameLoop->RenderCallbackClosure = p_closure;
};

void GameLoop_SetendOfFrameCallback(GameLoop* p_gameLoop, EndOfFrameCallback p_endOfFrameCallback, void* p_closure)
{
	p_gameLoop->EndofFrameCallback = p_endOfFrameCallback;
	p_gameLoop->EndOfFrameClosure = p_closure;
};

void GameLoop_Update(GameLoop* p_gameLoop)
{
	TimeClockPrecision l_currentTime = Clock_currentTime_mics();
	TimeClockPrecision l_elapsed = l_currentTime - p_gameLoop->PreviousUpdateTime_mics;

	if (l_elapsed > (TimeClockPrecision)(p_gameLoop->TimeBetweenUpdates_mics) * MAX_UPDATE_CALL_PER_FRAME)
	{
		l_elapsed = (TimeClockPrecision)(p_gameLoop->TimeBetweenUpdates_mics) * MAX_UPDATE_CALL_PER_FRAME;
	}

	p_gameLoop->PreviousUpdateTime_mics = l_currentTime;
	p_gameLoop->AccumulatedElapsedTime_mics += l_elapsed;

	if (p_gameLoop->AccumulatedElapsedTime_mics >= p_gameLoop->TimeBetweenUpdates_mics) {

		p_gameLoop->NewFrameCallback(p_gameLoop->NewFrameCallbackClosure);

		p_gameLoop->UpdateCallback(p_gameLoop->UpdateCallbackClosure, p_gameLoop->TimeBetweenUpdates_mics * 0.000001f);
		p_gameLoop->EndOfUpdateCallback(p_gameLoop->EndOfUpdateCallbackClosure);

		p_gameLoop->RenderCallback(p_gameLoop->RenderCallbackClosure);

		//TODO -> having a more precise loop (while delta < max  {} delta -= max) but preventing some piece of code to run twice.
		p_gameLoop->AccumulatedElapsedTime_mics = 0;
	}

	p_gameLoop->EndofFrameCallback(p_gameLoop->EndOfFrameClosure);
};