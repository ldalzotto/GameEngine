#pragma once

#include "Functional/Callback/ObserverT.hpp"
#include "Functional/Optional/OptionalT.hpp"

extern "C"
{
#include "v2/_interface/MatrixC_def.h"
}

#include <vector>
#include <string>

#ifdef _WIN32
#include "Include/PlatformInclude.hpp"
namespace _RenderV2
{
	struct WindowHandle
	{
		HWND Window;
	};

	struct Texture3C;
}
#endif

namespace _RenderV2
{
	struct WindowSize
	{
		uint32_t Width;
		uint32_t Height;
	};

	struct WindowState
	{
		bool AskedForClosed;
		_Core::OptionalT<WindowSize> HasResizedThisFrame;
		Texture3C* PendingPresentingTexture;
	};

	extern const uint32_t WINDOW_WIDTH;
	extern const uint32_t WINDOW_HEIGHT;

	struct Window
	{
		WindowHandle Handle;
		WindowSize WindowSize;

		WindowState WindowState;

		MATRIX3F WindowToGraphicsAPIPixelCoordinates;
		MATRIX4F GraphicsAPIToWindowPixelCoordinates;

		_Core::ObserverT<void> OnWindowSizeChanged;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);
	bool Window_consumeSizeChangeEvent(Window* p_window);

	WindowSize Window_getSize(Window* p_window);
	void Window_presentTexture(Window* p_window,  Texture3C* p_texture);
}