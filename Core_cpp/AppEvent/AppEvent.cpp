#include "AppEvent.hpp"


namespace _Core
{
	void initPlatformSpecificEventCallback();
	void poolEventPlatformSepcific();

	AppEventParams GlobalAppParams{};
	ObserverT<AppEvent_Header> EventDispatcher{};
	ObserverT<PlatformSpecificGenericEvent> EventPoolHook;

	void AppEvent_initialize()
	{
		ObserverT_alloc(&EventDispatcher); ObserverT_alloc(&EventPoolHook);
		initPlatformSpecificEventCallback();
	};

	void AppEvent_pool()
	{
		poolEventPlatformSepcific();
	};

	void AppEvent_free()
	{
		ObserverT_free(&EventDispatcher); ObserverT_free(&EventPoolHook);
	};
}

#ifdef _WIN32

namespace _Core
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void poolEventPlatformSepcific()
	{
		MSG msg{};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};
	};

	void initPlatformSpecificEventCallback()
	{
		GlobalAppParams.hInstance = GetModuleHandle(0);

		LPCSTR CLASS_NAME = "GameEngine";
		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GlobalAppParams.hInstance;
		wc.lpszClassName = CLASS_NAME;
		wc.hCursor = LoadCursor(GlobalAppParams.hInstance, IDC_ARROW);

		RegisterClass(&wc);
	};

	void Window_handleGenericEventHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (EventPoolHook.Subjects.Size > 0)
		{
			Window_handleGenericEventHook(hwnd, uMsg, wParam, lParam);
		}

		switch (uMsg)
		{
		case WM_CLOSE:
		{
			WindowEvent l_windowEvent{ {AppEventType::WINDOW_CLOSE}, hwnd };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_windowEvent);
			return 0;
		}
		case WM_SIZE:
		{
			INT l_width =  LOWORD(lParam);
			INT l_height = HIWORD(lParam);

			WindowResizeEvent l_windowResizeEvent{ {AppEventType::WINDOW_RESIZE}, hwnd, l_width, l_height };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_windowResizeEvent);
			return 0;
		}
		case WM_KEYDOWN:
		{
			InputKeyEvent l_inputKeyEvent{ {AppEventType::INPUT_KEY_EVENT}, wParam , InputKeyEventType::DOWN };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
			return 0;
		}
		case WM_KEYUP:
		{
			InputKeyEvent l_inputKeyEvent{ {AppEventType::INPUT_KEY_EVENT}, wParam , InputKeyEventType::UP };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			InputKeyEvent l_inputKeyEvent{ {AppEventType::INPUT_KEY_EVENT}, WM_LBUTTONDOWN , InputKeyEventType::DOWN };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
			return 0;
		}
		break;
		case WM_LBUTTONUP:
		{
			InputKeyEvent l_inputKeyEvent{ {AppEventType::INPUT_KEY_EVENT}, WM_LBUTTONUP , InputKeyEventType::UP };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);

			InputMouseEvent l_inputMouseEvent{ {AppEventType::INPUT_MOUSE_EVENT}, xPos, yPos };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputMouseEvent);
			return 0;
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		return 0;
	//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};

	void Window_handleGenericEventHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PlatformSpecificGenericEvent l_genericEvent;
		l_genericEvent.hwnd = hwnd; l_genericEvent.uMsg = uMsg; l_genericEvent.wParam = wParam; l_genericEvent.lParam = lParam;
		_Core::ObserverT_broadcast(&EventPoolHook, &l_genericEvent);
	};
}

#endif
