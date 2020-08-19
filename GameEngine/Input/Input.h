#pragma once

#include "InputGlobalEvent.hpp"

#include "Functional/Optional/OptionalT.hpp"

extern "C"
{
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/VectorC_def.h"
}

#include <queue>

namespace _RenderV2
{
	struct Window;
}

namespace _Core
{
	struct Log;
}

namespace _GameEngine::_Input
{
	enum KeyStateFlag
	{
		NONE = 0x01,
		PRESSED_THIS_FRAME = 0x02,
		PRESSED = 0x04,
		RELEASED_THIS_FRAME = 0x08,
		KEY_DOWN = PRESSED_THIS_FRAME | PRESSED,
		KEY_UP = RELEASED_THIS_FRAME | NONE
	};

	struct InputMouse
	{
		bool MouseEnabled;
		float MouseSentitivityperPixel = 0.01f;

		_Core::OptionalT< InputMouseGlobalEvent > CurrentInputMouseEvent;
		VECTOR2D ScreenPosition;

		VECTOR2D LastFrameMouseAbsoluteScreenPosition;
		VECTOR2D MouseDelta;
	};

	SEGMENT_VECTOR2F InputMouse_getMouseDeltaScreenPosition(InputMouse* p_inputMouse);

	struct Input
	{
		_RenderV2::Window* Window;
		::_Core::Log* Log;

		std::vector<KeyStateFlag> InputState;
		std::queue<InputGlobalEvent> InputEventsLastFrame;
		std::vector<InputKey> InputKeysReleasedThisFrame;
		std::vector<InputKey> InputKeysJustPressedThisFrame;
		InputMouse InputMouse;
	};

	void Input_build(Input* p_input, _RenderV2::Window* p_window, ::_Core::Log* Log);
	void Input_free(Input* p_input);

	bool Input_getState(Input* p_input, InputKey p_key, KeyStateFlag p_keyStateFlag);
	void Input_newFrame(Input* p_input);

	//We make this call to flush "JUST PRESSED" events. That way, we ensure that this event is set for only one "update loop" frame.
	// void Input_updateAfterUpdateFrame(Input* p_input);
}