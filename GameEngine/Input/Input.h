#pragma once

#include "InputGlobalEvent.hpp"

#include "Functional/Optional/OptionalT.hpp"
#include "v2/Vector/Vector.hpp"
#include "v2/Segment/SegmentV2.hpp"

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
		_MathV2::Vector2<double> ScreenPosition;

		_MathV2::Vector2<double> LastFrameMouseAbsoluteScreenPosition;
		_MathV2::Vector2<double> MouseDelta;
	};

	_MathV2::SegmentV2<2, float> InputMouse_getMouseDeltaScreenPosition(InputMouse* p_inputMouse);

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
	void Input_update(Input* p_input);
}