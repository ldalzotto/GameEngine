#pragma once

#include "Includes/GLFWIncludes.h"

#include "v2/Vector/Vector.hpp"
#include "v2/Segment/SegmentV2.hpp"

#include <unordered_map>
#include <queue>
#include <vector>

namespace _GameEngine::_Render
{
	struct Window;
}

namespace _Core
{
	struct Log;
}

namespace _GameEngine::_Input
{
	struct InputEvent
	{
		int KeyCode;
		int Action;
	};

	enum KeyStateFlag
	{
		NONE = 0x01,
		PRESSED_THIS_FRAME = 0x02,
		PRESSED = 0x04,
		RELEASED_THIS_FRAME = 0x08,
		KEY_DOWN = PRESSED_THIS_FRAME | PRESSED,
		KEY_UP = RELEASED_THIS_FRAME | NONE
	};

	enum InputKey : uint8_t
	{
		SPACE = 0,
		APOSTROPHE = 1,
		COMMA = 2,
		MINUS = 3,
		PERIOD = 4,
		SLASH = 5,
		ZERO = 6,
		ONE = 7,
		TWO = 8,
		THREE = 9,
		FOUR = 10,
		FIVE = 11,
		SIX = 12,
		SEVEN = 13,
		EIGHT = 14,
		NINE = 15,
		SEMICOLON = 16,
		EQUAL = 17,
		A = 18,
		B = 19,
		C = 20,
		D = 21,
		E = 22,
		F = 23,
		G = 24,
		H = 25,
		I = 26,
		J = 27,
		K = 28,
		L = 29,
		M = 30,
		N = 31,
		O = 32,
		P = 33,
		Q = 34,
		R = 35,
		S = 36,
		T = 37,
		U = 38,
		V = 39,
		W = 40,
		X = 41,
		Y = 42,
		Z = 43,
		LEFT_BRACKET = 44,
		BACKSLASH = 45,
		RIGHT_BRACKET = 46,
		GRAVE_ACCENT = 47,
		WORLD_1 = 48,
		WORLD_2 = 49,
		ESCAPE = 50,
		ENTER = 51,
		TAB = 52,
		BACKSPACE = 53,
		INSERT = 54,
		DELETE = 55,
		RIGHT = 56,
		LEFT = 57,
		DOWN = 58,
		UP = 59,
		PAGE_UP = 60,
		PAGE_DOWN = 61,
		HOME = 62,
		END = 63,
		CAPS_LOCK = 64,
		SCROLL_LOCK = 65,
		NUM_LOCK = 66,
		PRINT_SCREEN = 67,
		PAUSE = 68,
		F1 = 69,
		F2 = 70,
		F3 = 71,
		F4 = 72,
		F5 = 73,
		F6 = 74,
		F7 = 75,
		F8 = 76,
		F9 = 77,
		F10 = 78,
		F11 = 79,
		F12 = 80,
		F13 = 81,
		F14 = 82,
		F15 = 83,
		F16 = 84,
		F17 = 85,
		F18 = 86,
		F19 = 87,
		F20 = 88,
		F21 = 89,
		F22 = 90,
		F23 = 91,
		F24 = 92,
		F25 = 93,
		KP_ZERO = 94,
		KP_ONE = 95,
		KP_TWO = 96,
		KP_THREE = 97,
		KP_FOUR = 98,
		KP_FIVE = 99,
		KP_SIX = 100,
		KP_SEVEN = 101,
		KP_EIGHT = 102,
		KP_NINE = 103,
		KP_DECIMAL = 104,
		KP_DIVIDE = 105,
		KP_MULTIPLY = 106,
		KP_SUBTRACT = 107,
		KP_ADD = 108,
		KP_ENTER = 109,
		KP_EQUAL = 110,
		LEFT_SHIFT = 111,
		LEFT_CONTROL = 112,
		LEFT_ALT = 113,
		LEFT_SUPER = 114,
		RIGHT_SHIFT = 115,
		RIGHT_CONTROL = 116,
		RIGHT_ALT = 117,
		RIGHT_SUPER = 118,
		MENU = 119,

		MOUSE_BUTTON_1 = 120,
		MOUSE_BUTTON_2 = 121,
		MOUSE_BUTTON_3 = 122,
		MOUSE_BUTTON_4 = 123,
		MOUSE_BUTTON_5 = 124,
		MOUSE_BUTTON_6 = 125,
		MOUSE_BUTTON_7 = 126,
		MOUSE_BUTTON_8 = 127,
		MOUSE_BUTTON_LAST = 128,
		MOUSE_BUTTON_LEFT = 129,
		MOUSE_BUTTON_RIGHT = 130,
		MOUSE_BUTTON_MIDDLE = 131,
		LAST
	};

	struct InputMouse
	{
		bool MouseEnabled;
		float MouseSentitivityperPixel = 0.01f;
		_MathV2::Vector2<double> ScreenPosition;

		_MathV2::Vector2<double> LastFrameMouseAbsoluteScreenPosition;
		_MathV2::Vector2<double> MouseDelta;
	};

	_MathV2::SegmentV2<2, float> InputMouse_getMouseDeltaScreenPosition(InputMouse* p_inputMouse);

	struct Input
	{
		_Render::Window* Window;
		::_Core::Log* Log;

		std::vector<KeyStateFlag> InputState;
		std::queue<InputEvent> InputEventsLastFrame;
		std::vector<InputKey> InputKeysReleasedThisFrame;
		// We store juste pressed events for mouse because glfw doesn't trigger hold events. Thus, we manually trigger them.
		std::vector<int> MouseInputKeyCodeJustPressedThisFrame;
		std::unordered_map<int, InputKey> GLFWKeyToInputKeyLookup;
		InputMouse InputMouse;
	};

	void Input_build(Input* p_input, _Render::Window* p_window, ::_Core::Log* Log);
	void Input_free(Input* p_input);

	bool Input_getState(Input* p_input, InputKey p_key, KeyStateFlag p_keyStateFlag);
	void Input_update(Input* p_input);
}