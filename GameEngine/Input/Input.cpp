#include "Input.h"

#include <iostream>
#include <math.h>


extern "C"
{
#include "v2/Math.h"
#include "v2/_interface/VectorC.h"
#include "Objects/Window/Window.h"
}

namespace _GameEngine::_Input
{
	Input* InputInstance;

	void OnMouseMoveEvent(Input* p_input, InputMouseGlobalEvent* p_event);
	void OnKeyEvent(Input* p_input, InputGlobalEvent* p_event);

	bool InputKey_isMouse(InputKey p_inputKey);

	Segment_Vector2f InputMouse_getMouseDeltaScreenPosition(InputMouse* p_inputMouse)
	{
		Segment_Vector2f l_segment;
		l_segment.Begin = { (float)p_inputMouse->LastFrameMouseAbsoluteScreenPosition.x, (float)p_inputMouse->LastFrameMouseAbsoluteScreenPosition.y };
		l_segment.End = { (float)p_inputMouse->ScreenPosition.x, (float)p_inputMouse->ScreenPosition.y };
		return l_segment;
	};

	void Input_build(Input* p_input, Window* p_window, ::_Core::Log* Log)
	{
		InputInstance = p_input;
		p_input->Window = p_window;
		p_input->Log = Log;

		p_input->InputMouse.MouseEnabled = true;

		InputKey l_inputKey = (InputKey)0;
		while (l_inputKey != InputKey::LAST)
		{
			p_input->InputState.push_back(KeyStateFlag::NONE);
			l_inputKey = (InputKey)((uint8_t)l_inputKey + 1);
		}

		_Core::CallbackT<Input, InputGlobalEvent> l_inputKeyCallback{ OnKeyEvent, p_input };
		_Core::CallbackT<Input, InputMouseGlobalEvent> l_inputMouseCallback{ OnMouseMoveEvent, p_input };
		_Core::ObserverT_register(&InputGlobalEvent_dispatcher, (_Core::CallbackT<void, InputGlobalEvent>*) & l_inputKeyCallback);
		_Core::ObserverT_register(&InputMouseGlobalEvent_dispatcher, (_Core::CallbackT<void, InputMouseGlobalEvent>*) & l_inputMouseCallback);

	};

	void Input_free(Input* p_input)
	{
		InputInstance = nullptr;
	};

	void Input_consumeLastFrameEvents(_GameEngine::_Input::Input* p_input)
	{
		while (p_input->InputEventsLastFrame.size() > 0)
		{
			InputGlobalEvent l_inputEvent = p_input->InputEventsLastFrame.front();
			KeyStateFlag* l_oldStateFlag = &p_input->InputState.at(static_cast<int>(l_inputEvent.Key));

			if ((*l_oldStateFlag & (KeyStateFlag::PRESSED | KeyStateFlag::PRESSED_THIS_FRAME)) && l_inputEvent.Type == InputGlobalEventType::RELEASED)
			{
				*l_oldStateFlag = KeyStateFlag::RELEASED_THIS_FRAME;
				p_input->InputKeysReleasedThisFrame.emplace_back(l_inputEvent.Key);
			}
			else if ((*l_oldStateFlag & KeyStateFlag::PRESSED_THIS_FRAME) && (l_inputEvent.Type == InputGlobalEventType::REPEAT || l_inputEvent.Type == InputGlobalEventType::PRESSED))
			{
				*l_oldStateFlag = KeyStateFlag::PRESSED;
			}
			else if ((*l_oldStateFlag & (KeyStateFlag::NONE | KeyStateFlag::RELEASED_THIS_FRAME)) && l_inputEvent.Type == InputGlobalEventType::PRESSED)
			{
				*l_oldStateFlag = KeyStateFlag::PRESSED_THIS_FRAME;
				p_input->InputKeysJustPressedThisFrame.emplace_back(l_inputEvent.Key);
			}

			p_input->InputEventsLastFrame.pop();
		}
	};


	void Input_newFrame(Input* p_input)
	{
		if (p_input->InputKeysReleasedThisFrame.size() > 0)
		{
			for (InputKey& l_inputKeyReleasedLastFrame : p_input->InputKeysReleasedThisFrame)
			{
				p_input->InputState.at(static_cast<int>(l_inputKeyReleasedLastFrame)) = KeyStateFlag::NONE;
			}

			p_input->InputKeysReleasedThisFrame.clear();
		}

		while (p_input->InputKeysJustPressedThisFrame.size() > 0)
		{
			InputKey l_inputKeyCode = p_input->InputKeysJustPressedThisFrame.at(p_input->InputKeysJustPressedThisFrame.size() - 1);
			p_input->InputEventsLastFrame.push(InputGlobalEvent{ l_inputKeyCode, InputGlobalEventType::REPEAT });
			p_input->InputKeysJustPressedThisFrame.pop_back();
		}

		Input_consumeLastFrameEvents(p_input);

		if (p_input->InputMouse.CurrentInputMouseEvent.HasValue)
		{
			p_input->InputMouse.LastFrameMouseAbsoluteScreenPosition = p_input->InputMouse.ScreenPosition;
			p_input->InputMouse.ScreenPosition.x = p_input->InputMouse.CurrentInputMouseEvent.Value.X;
			p_input->InputMouse.ScreenPosition.y = p_input->InputMouse.CurrentInputMouseEvent.Value.Y;

			std::cout << "X" << p_input->InputMouse.ScreenPosition.x << "  Y  " << p_input->InputMouse.ScreenPosition.y << std::endl;

			Vec_Min_2d_2d(&p_input->InputMouse.ScreenPosition, &p_input->InputMouse.LastFrameMouseAbsoluteScreenPosition, &p_input->InputMouse.MouseDelta);

			auto l_windowDimensions = Window_getSize(p_input->Window);

			p_input->InputMouse.ScreenPosition.x = Math_max(Math_min(p_input->InputMouse.ScreenPosition.x, static_cast<double>(l_windowDimensions.Width)), 0.0f);
			p_input->InputMouse.ScreenPosition.y = Math_max(Math_min(p_input->InputMouse.ScreenPosition.y, static_cast<double>(l_windowDimensions.Height)), 0.0f);

			p_input->InputMouse.CurrentInputMouseEvent.HasValue = false;
		}
		else
		{
			p_input->InputMouse.MouseDelta = { 0.0, 0.0 };
		}

	}

	/*
	void Input_updateAfterUpdateFrame(Input* p_input)
	{
		while (p_input->InputKeysJustPressedThisFrame.size() > 0)
		{
			InputKey l_inputKeyCode = p_input->InputKeysJustPressedThisFrame.at(p_input->InputKeysJustPressedThisFrame.size() - 1);
			p_input->InputEventsLastFrame.push(InputGlobalEvent{ l_inputKeyCode, InputGlobalEventType::REPEAT });
			p_input->InputKeysJustPressedThisFrame.pop_back();
		}

		Input_consumeLastFrameEvents(p_input);
	};
	*/
	void OnMouseMoveEvent(Input* p_input, InputMouseGlobalEvent* p_event)
	{
		p_input->InputMouse.CurrentInputMouseEvent.Value = *p_event;
		p_input->InputMouse.CurrentInputMouseEvent.HasValue = true;
	};

	void OnKeyEvent(Input* p_input, InputGlobalEvent* p_event)
	{
		p_input->InputEventsLastFrame.push(*p_event);
	};


	bool Input_getState(Input* p_input, InputKey p_key, KeyStateFlag p_keyStateFlag)
	{
		return p_input->InputState.at(static_cast<int>(p_key)) & p_keyStateFlag;
	};

	bool InputKey_isMouse(InputKey p_inputKey)
	{
		return
			p_inputKey == InputKey::MOUSE_BUTTON_1 ||
			p_inputKey == InputKey::MOUSE_BUTTON_2 ||
			p_inputKey == InputKey::MOUSE_BUTTON_3 ||
			p_inputKey == InputKey::MOUSE_BUTTON_4 ||
			p_inputKey == InputKey::MOUSE_BUTTON_5 ||
			p_inputKey == InputKey::MOUSE_BUTTON_6 ||
			p_inputKey == InputKey::MOUSE_BUTTON_7 ||
			p_inputKey == InputKey::MOUSE_BUTTON_8 ||
			p_inputKey == InputKey::MOUSE_BUTTON_LAST ||
			p_inputKey == InputKey::MOUSE_BUTTON_RIGHT ||
			p_inputKey == InputKey::MOUSE_BUTTON_MIDDLE;
	};
}