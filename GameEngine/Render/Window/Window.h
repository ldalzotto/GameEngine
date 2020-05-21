#pragma once

#include "Render/Includes/GLFWIncludes.h"

#include <vector>
#include <string>

namespace _GameEngine::_Render::_Window
{
	struct WindowSize
	{
		int Width;
		int Height;
	};

	struct Window
	{
		GLFWwindow* Window;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);

	WindowSize getWindowSize(Window* p_window);

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window);
} // namespace _GameEngine