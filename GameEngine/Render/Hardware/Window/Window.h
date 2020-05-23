#pragma once

#include "Utils/Observer/Observer.h"
using namespace _GameEngine::_Utils;

#include "Render/Includes/GLFWIncludes.h"

#include <vector>
#include <string>

namespace _GameEngine::_Render::_Hardware::_Window
{
	struct WindowSize
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct Window
	{
		GLFWwindow* Window;
		_Observer::Observer OnWindowSizeChanged;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);

	WindowSize getWindowSize(Window* p_window);

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window);
} // namespace _GameEngine