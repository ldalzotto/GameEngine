#include "Window.hpp"

#include "AppEvent/AppEvent.hpp"

#include <unordered_map>
#include <stdexcept>

#include "Log/LogFormatting.hpp"
#include "v2/Matrix/MatrixMath.hpp"

namespace _RenderV2
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

		_MathV2::Vector4<float> l_col_1, l_col_2, l_col_3, l_col_4;
		l_col_1 = { (float)p_window->WindowSize.Width / 2.0f , 0.0f, 0.0f, 0.0f };
		l_col_2 = { 0.0f,(float)p_window->WindowSize.Height / 2.0f, 0.0f, 0.0f };
		l_col_3 = { (float)p_window->WindowSize.Width * 0.5f,0.0f, 0.0f, 0.0f };
		l_col_4 = { 0.0f,(float)p_window->WindowSize.Height * 0.5f, 0.0f, 0.0f };
		p_window->GraphicsAPIToWindowPixelCoordinates = _MathV2::MatrixM::build(&l_col_1, &l_col_2, &l_col_3, &l_col_4);

	};

	void Window_init(Window* p_window)
	{
		_Core::ObserverT_alloc(&p_window->OnWindowSizeChanged);

		p_window->WindowSize.Width = WINDOW_WIDTH;
		p_window->WindowSize.Height = WINDOW_HEIGHT;
		Window_updateScreeToGraphicsAPIPixelCoordinates(p_window);
		windowHookToGlobalEvents(p_window);
		windowPlatforwSpecific_open(p_window);
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
				p_window->WindowSize.Width = l_windowResizeEvent->Width;
				p_window->WindowSize.Height = l_windowResizeEvent->Height;
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

namespace _RenderV2
{
	void windowPlatforwSpecific_open(Window* p_window)
	{
		p_window->Handle.Window = CreateWindowEx(0, "GameEngine", "Learn to Program Windows", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, p_window->WindowSize.Width, p_window->WindowSize.Height,
			NULL, NULL, _Core::GlobalAppParams.hInstance, NULL);
		ShowWindow(p_window->Handle.Window, SW_SHOW);
	};

	void windowPlatforwSpecific_close(Window* p_window)
	{
		DestroyWindow(p_window->Handle.Window);
	};
}

#endif //  _WIN32

