#pragma once

#include "Functional/Callback/ObserverT.hpp"

#include "v2/Matrix/Matrix.hpp"

#include <vector>
#include <string>

#ifdef _WIN32
#include "Include/PlatformInclude.hpp"
namespace _GameEngine::_Render
{
	struct WindowHandle
	{
		HWND Window;
	};
}
#endif

namespace _GameEngine::_Render
{
	struct WindowSize
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct WindowState
	{
		bool AskedForClosed;
	};

	struct Window
	{
		WindowHandle Handle;
		WindowSize WindowSize;
		
		WindowState WindowState;

		_MathV2::Matrix3x3<float> WindowToGraphicsAPIPixelCoordinates;
		
		_Core::ObserverT<void> OnWindowSizeChanged;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);

	WindowSize Window_getSize(Window* p_window);

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window);
} // namespace _GameEngine

