#include "Input.h"

#include <math.h>
#include "VulkanObjects/Hardware/Window/Window.h"

#include "Math/Math.h"
#include "MyLog/MyLog.h"

namespace _GameEngine::_Input
{
	Input* InputInstance;

	void OnKeyEvent(GLFWwindow* p_window, int key, int scancode, int action, int mods);
	void OnMouseEvent(GLFWwindow* window, int button, int action, int mods);

	void initializeGLFWLookup(Input* p_input);

	void Input_build(Input* p_input, _Render::Window* p_window, _Log::MyLog* Log)
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

		initializeGLFWLookup(p_input);

		glfwSetKeyCallback(p_window->Window, OnKeyEvent);
		glfwSetMouseButtonCallback(p_window->Window, OnMouseEvent);
		
	};

	void Input_free(Input* p_input)
	{
		InputInstance = nullptr;
	};

	void Input_update(Input* p_input)
	{
		if (p_input->InputKeysReleasedThisFrame.size() > 0)
		{
			for (InputKey& l_inputKeyReleasedLastFrame : p_input->InputKeysReleasedThisFrame)
			{
				p_input->InputState.at(l_inputKeyReleasedLastFrame) = KeyStateFlag::NONE;
			}

			p_input->InputKeysReleasedThisFrame.clear();
		}

		while (p_input->InputEventsLastFrame.size() > 0)
		{
			InputEvent l_inputEvent = p_input->InputEventsLastFrame.front();
			
			InputKey l_inputKey = p_input->GLFWKeyToInputKeyLookup.at(l_inputEvent.KeyCode);
			KeyStateFlag* l_oldStateFlag = &p_input->InputState.at(l_inputKey);
			
			if ((*l_oldStateFlag & (KeyStateFlag::PRESSED | KeyStateFlag::PRESSED_THIS_FRAME)) && l_inputEvent.Action == GLFW_RELEASE)
			{
				*l_oldStateFlag = KeyStateFlag::RELEASED_THIS_FRAME;
				p_input->InputKeysReleasedThisFrame.emplace_back(l_inputKey);
			}
			else if ((*l_oldStateFlag & KeyStateFlag::PRESSED_THIS_FRAME) && (l_inputEvent.Action == GLFW_REPEAT || l_inputEvent.Action == GLFW_PRESS))
			{
				*l_oldStateFlag = KeyStateFlag::PRESSED;
			}
			else if ((*l_oldStateFlag & (KeyStateFlag::NONE | KeyStateFlag::RELEASED_THIS_FRAME)) && l_inputEvent.Action == GLFW_PRESS)
			{
				*l_oldStateFlag = KeyStateFlag::PRESSED_THIS_FRAME;
			}
			
			p_input->InputEventsLastFrame.pop();
		}

		if (p_input->InputMouse.MouseEnabled)
		{
			glfwGetCursorPos(p_input->Window->Window, &p_input->InputMouse.ScreenPosition.x, &p_input->InputMouse.ScreenPosition.y);
			auto l_windowDimensions = _Render::Window_getSize(p_input->Window);

			p_input->InputMouse.ScreenPosition.x = _Math::Math_max(_Math::Math_min(p_input->InputMouse.ScreenPosition.x, static_cast<double>(l_windowDimensions.Width)), 0.0f);
			p_input->InputMouse.ScreenPosition.y = _Math::Math_max(_Math::Math_min(p_input->InputMouse.ScreenPosition.y, static_cast<double>(l_windowDimensions.Height)), 0.0f);
		}
	};

	void OnKeyEvent(GLFWwindow* p_window, int key, int scancode, int action, int mods)
	{
		InputEvent l_inputEvent{};
		l_inputEvent.KeyCode = key;
		l_inputEvent.Action = action;
		InputInstance->InputEventsLastFrame.push(l_inputEvent);
	};

	void OnMouseEvent(GLFWwindow* window, int button, int action, int mods)
	{
		InputEvent l_inputEvent{};
		l_inputEvent.KeyCode = button;
		l_inputEvent.Action = action;
		InputInstance->InputEventsLastFrame.push(l_inputEvent);
	};

	bool Input_getState(Input* p_input, InputKey p_key, KeyStateFlag p_keyStateFlag)
	{
		return p_input->InputState.at(p_key) & p_keyStateFlag;
	};

	void initializeGLFWLookup(Input* p_input)
	{
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_SPACE, InputKey::SPACE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_APOSTROPHE, InputKey::APOSTROPHE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_COMMA, InputKey::COMMA);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_MINUS, InputKey::MINUS);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_PERIOD, InputKey::PERIOD);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_SLASH, InputKey::SLASH);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_0, InputKey::ZERO);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_1, InputKey::ONE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_2, InputKey::TWO);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_3, InputKey::THREE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_4, InputKey::FOUR);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_5, InputKey::FIVE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_6, InputKey::SIX);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_7, InputKey::SEVEN);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_8, InputKey::EIGHT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_9, InputKey::NINE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_SEMICOLON, InputKey::SEMICOLON);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_EQUAL, InputKey::EQUAL);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_A, InputKey::A);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_B, InputKey::B);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_C, InputKey::C);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_D, InputKey::D);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_E, InputKey::E);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F, InputKey::F);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_G, InputKey::G);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_H, InputKey::H);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_I, InputKey::I);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_J, InputKey::J);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_K, InputKey::K);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_L, InputKey::L);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_M, InputKey::M);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_N, InputKey::N);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_O, InputKey::O);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_P, InputKey::P);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_Q, InputKey::Q);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_R, InputKey::R);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_S, InputKey::S);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_T, InputKey::T);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_U, InputKey::U);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_V, InputKey::V);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_W, InputKey::W);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_X, InputKey::X);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_Y, InputKey::Y);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_Z, InputKey::Z);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_LEFT_BRACKET, InputKey::LEFT_BRACKET);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_BACKSLASH, InputKey::BACKSLASH);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_RIGHT_BRACKET, InputKey::RIGHT_BRACKET);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_GRAVE_ACCENT, InputKey::GRAVE_ACCENT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_WORLD_1, InputKey::WORLD_1);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_WORLD_2, InputKey::WORLD_2);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_ESCAPE, InputKey::ESCAPE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_ENTER, InputKey::ENTER);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_TAB, InputKey::TAB);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_BACKSPACE, InputKey::BACKSPACE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_INSERT, InputKey::INSERT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_DELETE, InputKey::DELETE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_RIGHT, InputKey::RIGHT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_LEFT, InputKey::LEFT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_DOWN, InputKey::DOWN);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_UP, InputKey::UP);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_PAGE_UP, InputKey::PAGE_UP);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_PAGE_DOWN, InputKey::PAGE_DOWN);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_HOME, InputKey::HOME);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_END, InputKey::END);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_CAPS_LOCK, InputKey::CAPS_LOCK);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_SCROLL_LOCK, InputKey::SCROLL_LOCK);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_NUM_LOCK, InputKey::NUM_LOCK);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_PRINT_SCREEN, InputKey::PRINT_SCREEN);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_PAUSE, InputKey::PAUSE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F1, InputKey::F1);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F2, InputKey::F2);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F3, InputKey::F3);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F4, InputKey::F4);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F5, InputKey::F5);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F6, InputKey::F6);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F7, InputKey::F7);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F8, InputKey::F8);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F9, InputKey::F9);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F10, InputKey::F10);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F11, InputKey::F11);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F12, InputKey::F12);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F13, InputKey::F13);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F14, InputKey::F14);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F15, InputKey::F15);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F16, InputKey::F16);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F17, InputKey::F17);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F18, InputKey::F18);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F19, InputKey::F19);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F20, InputKey::F20);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F21, InputKey::F21);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F22, InputKey::F22);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F23, InputKey::F23);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F24, InputKey::F24);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_F25, InputKey::F25);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_0, InputKey::KP_ZERO);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_1, InputKey::KP_ONE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_2, InputKey::KP_TWO);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_3, InputKey::KP_THREE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_4, InputKey::KP_FOUR);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_5, InputKey::KP_FIVE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_6, InputKey::KP_SIX);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_7, InputKey::KP_SEVEN);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_8, InputKey::KP_EIGHT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_9, InputKey::KP_NINE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_DECIMAL, InputKey::KP_DECIMAL);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_DIVIDE, InputKey::KP_DIVIDE);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_MULTIPLY, InputKey::KP_MULTIPLY);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_SUBTRACT, InputKey::KP_SUBTRACT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_ADD, InputKey::KP_ADD);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_ENTER, InputKey::KP_ENTER);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_KP_EQUAL, InputKey::KP_EQUAL);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_LEFT_SHIFT, InputKey::LEFT_SHIFT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_LEFT_CONTROL, InputKey::LEFT_CONTROL);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_LEFT_ALT, InputKey::LEFT_ALT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_LEFT_SUPER, InputKey::LEFT_SUPER);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_RIGHT_SHIFT, InputKey::RIGHT_SHIFT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_RIGHT_CONTROL, InputKey::RIGHT_CONTROL);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_RIGHT_ALT, InputKey::RIGHT_ALT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_RIGHT_SUPER, InputKey::RIGHT_SUPER);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_KEY_MENU, InputKey::MENU);

		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_1, InputKey::MOUSE_BUTTON_1);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_2, InputKey::MOUSE_BUTTON_2);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_3, InputKey::MOUSE_BUTTON_3);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_4, InputKey::MOUSE_BUTTON_4);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_5, InputKey::MOUSE_BUTTON_5);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_6, InputKey::MOUSE_BUTTON_6);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_7, InputKey::MOUSE_BUTTON_7);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_8, InputKey::MOUSE_BUTTON_8);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_LAST, InputKey::MOUSE_BUTTON_LAST);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_LEFT, InputKey::MOUSE_BUTTON_LEFT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_RIGHT, InputKey::MOUSE_BUTTON_RIGHT);
		p_input->GLFWKeyToInputKeyLookup.emplace(GLFW_MOUSE_BUTTON_MIDDLE, InputKey::MOUSE_BUTTON_MIDDLE);
	};
}