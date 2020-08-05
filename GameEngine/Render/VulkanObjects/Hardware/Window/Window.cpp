#include "Window.h"

#include "Includes/VulkanIncludes.h"
#include "AppEvent/AppEvent.hpp"

#include <unordered_map>
#include <stdexcept>

#include "Log/LogFormatting.hpp"
#include "v2/Matrix/MatrixMath.hpp"

namespace _GameEngine::_Render
{
	void windowHookToGlobalEvents(Window* p_window);
	void windowPlatforwSpecific_open(Window* p_window);
	void windowPlatforwSpecific_close(Window* p_window);

	const uint32_t WINDOW_WIDTH = 800;
	const uint32_t WINDOW_HEIGHT = 600;

	const char* WINDOW_ERROR_NOT_INITIALIZED = "The Window->Window is not initialized.";

	// void window_size_callback(GLFWwindow* window, int width, int height);
	void Window_updateScreeToGraphicsAPIPixelCoordinates(Window* p_window)
	{
		_MathV2::Vector3<float> l_right, l_up, l_forward;
		l_right = { 2.0f / (float)p_window->WindowSize.Width , 0.0f, 0.0f };
		l_up = { 0.0f, 2.0f / (float)p_window->WindowSize.Height, 0.0f };
		l_forward = { -1.0f, -1.0f, 0.0f };
		p_window->WindowToGraphicsAPIPixelCoordinates = _MathV2::MatrixM::build(&l_right, &l_up, &l_forward);
	};

	void Window_init(Window* p_window)
	{
		_Core::ObserverT_alloc(&p_window->OnWindowSizeChanged);

		p_window->WindowSize.Width = WINDOW_WIDTH;
		p_window->WindowSize.Height = WINDOW_HEIGHT;
		Window_updateScreeToGraphicsAPIPixelCoordinates(p_window);
		windowPlatforwSpecific_open(p_window);
		windowHookToGlobalEvents(p_window);
	}

	void Window_closeWindow(Window* p_window)
	{
		_Core::ObserverT_free(&p_window->OnWindowSizeChanged);
		windowPlatforwSpecific_close(p_window);
		p_window->Handle.Window = nullptr;
	};

	bool Window_askedForClose(Window* p_window)
	{
		return p_window->WindowState.AskedForClosed;
	};

	WindowSize Window_getSize(Window* p_window)
	{
		return p_window->WindowSize;
	};

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window)
	{

#ifndef NDEBUG
		if (!p_window->Handle.Window)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE((char*)WINDOW_ERROR_NOT_INITIALIZED));
		}
#endif

		std::vector<char*> l_extensions = std::vector<char*>();
		l_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
		l_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
		return l_extensions;
	};

	void window_onGlobalEvent(Window* p_window, _Core::AppEvent_Header* p_eventHeader)
	{
		switch (p_eventHeader->EventType)
		{
		case _Core::AppEventType::WINDOW_CLOSE:
		{
			_Core::WindowEvent* l_windowEvent = (_Core::WindowEvent*)p_eventHeader;
			if (memcmp(&l_windowEvent->Window, &p_window->Handle.Window, sizeof(WindowHandle)) == 0)
			{
				p_window->WindowState.AskedForClosed = true;
			}
		}
		break;
		case _Core::AppEventType::WINDOW_RESIZE:
		{
			_Core::WindowResizeEvent* l_windowResizeEvent = (_Core::WindowResizeEvent*)p_eventHeader;
			if (memcmp(&l_windowResizeEvent->Window, &p_window->Handle.Window, sizeof(WindowHandle)) == 0)
			{
				Window_updateScreeToGraphicsAPIPixelCoordinates(p_window);
				_Core::ObserverT_broadcast(&p_window->OnWindowSizeChanged, (void*)nullptr);
			}
		}
		break;
		}
	}

	void windowHookToGlobalEvents(Window* p_window)
	{
		_Core::CallbackT<Window, _Core::AppEvent_Header> l_appGlobalEvetnCallback = { window_onGlobalEvent, p_window };
		_Core::ObserverT_register(&_Core::EventDispatcher, (_Core::CallbackT<void, _Core::AppEvent_Header>*) & l_appGlobalEvetnCallback);
	};

} // namespace _GameEngine

#ifdef  _WIN32

namespace _GameEngine::_Render
{
	void windowPlatforwSpecific_open(Window* p_window)
	{
		p_window->Handle.Window = CreateWindowEx(0, "GameEngine", "Learn to Program Windows", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, p_window->WindowSize.Width, p_window->WindowSize.Height,
			NULL, NULL, _Core::GlobalAppParams.hInstance, NULL);
		ShowWindow(p_window->Handle.Window, _Core::GlobalAppParams.nCmdShow);

		// SetCursor(LoadCursor(_Core::GlobalAppParams.hInstance, IDC_ARROW));
	};

	void windowPlatforwSpecific_close(Window* p_window)
	{
		DestroyWindow(p_window->Handle.Window);
	};
}

#endif //  _WIN32

