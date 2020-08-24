#pragma once

#include "InputGlobalEvent.h"
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Objects/Window/Window_def.h"
#include "Log/Log_def.h"

typedef unsigned char KeyStateFlag;
#define KeyStateFlag_NONE 0
#define KeyStateFlag_PRESSED_THIS_FRAME 1
#define KeyStateFlag_PRESSED 2
#define KeyStateFlag_RELEASED_THIS_FRAME 4
#define KeyStateFlag_KEY_DOWN (KeyStateFlag_PRESSED_THIS_FRAME | KeyStateFlag_PRESSED)
#define KeyStateFlag_KEY_UP (KeyStateFlag_RELEASED_THIS_FRAME | KeyStateFlag_NONE)

typedef struct Array_KeyStateFlag_TYP
{
	ARRAY_TYPE_DEFINITION(KeyStateFlag)
}Array_KeyStateFlag, * Array_KeyStateFlag_PTR;

typedef struct InputMouse_TYP
{
	bool MouseEnabled;
	float MouseSentitivityperPixel;

	Vector2d ScreenPosition;

	Vector2d LastFrameMouseAbsoluteScreenPosition;
	Vector2d MouseDelta;
}InputMouse, * InputMouse_PTR;

Segment_Vector2f InputMouse_getMouseDeltaScreenPosition(InputMouse* p_inputMouse);

typedef struct Input_TYP
{
	Window* Window;
	Log_PTR Log;

	Array_KeyStateFlag InputState;
	Array_InputGlobalEvent InputEventsLastFrame;
	Array_InputKey InputKeysReleasedThisFrame;
	Array_InputKey InputKeysJustPressedThisFrame;
	InputMouse InputMouse;
}Input, * Input_PTR;

void Input_build(Input* p_input, Window* p_window, Log_PTR Log);
void Input_free(Input* p_input);

bool Input_getState(Input* p_input, InputKey p_key, KeyStateFlag p_keyStateFlag);
void Input_newFrame(Input* p_input);

//We make this call to flush "JUST PRESSED" events. That way, we ensure that this event is set for only one "update loop" frame.
// void Input_updateAfterUpdateFrame(Input* p_input);	