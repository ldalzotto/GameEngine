#pragma once


#include "Functional/Callback/ObserverT.hpp"

#include "v2/Matrix/Matrix.hpp"

#include <vector>
#include <string>

namespace _GameEngine::_Render
{
	struct WindowSize
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct Window
	{
		// GLFWwindow* Window;
		WindowSize WindowSize;

		_MathV2::Matrix3x3<float> WindowToGraphicsAPIPixelCoordinates;
		_Core::ObserverT<void> OnWindowSizeChanged;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);

	WindowSize Window_getSize(Window* p_window);

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window);
} // namespace _GameEngine