#pragma once


typedef void (*NewFrameCallback)(void* p_closure);
typedef void (*UpdateCallback)(void* p_closure, float p_delta);
typedef void (*EndOfUpdateCallback)(void* p_closure);
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
typedef struct GameLoop_TYP
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

	EndOfUpdateCallback EndOfUpdateCallback;
	void* EndOfUpdateCallbackClosure;

	RenderCallback RenderCallback;
	void* RenderCallbackClosure;

	EndOfFrameCallback EndofFrameCallback;
	void* EndOfFrameClosure;
}GameLoop, * GameLoop_PTR;


void GameLoop_build(GameLoop* p_gameLoop, TimeClockPrecision p_timeBetweenUpdates_mics);
void GameLoop_free(GameLoop* p_gameLoop);

void GameLoop_SetnewFrameCallback(GameLoop* p_gameLoop, NewFrameCallback p_newFrameCallback, void* p_closure);
void GameLoop_SetupdateCallback(GameLoop* p_gameLoop, UpdateCallback p_updateCallback, void* p_closure);
void GameLoop_SetendOfUpdateCallback(GameLoop* p_gameLoop, EndOfFrameCallback p_endOfUpdateCallback, void* p_closure);
void GameLoop_SetrenderCallback(GameLoop* p_gameLoop, RenderCallback p_renderCallback, void* p_closure);
void GameLoop_SetendOfFrameCallback(GameLoop* p_gameLoop, EndOfFrameCallback p_endOfFrameCallback, void* p_closure);

/**
	@brief Execute callbacks for the fixed time rate.

	This function represents a single frame execution in the GameEngine.
*/
void GameLoop_Update(GameLoop* p_gameLoop);


