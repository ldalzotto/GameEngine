#include "AppEvent.h"

#include "Functional/Callback/Observer.h"


void initPlatformSpecificEventCallback();
void poolEventPlatformSepcific();

AppEventParams GlobalAppParams = {0};
AppEventObserver EventDispatcher = {0};

void AppEventObserver_Register(AppEventObserver* p_observer, void(*p_callback)(void*, AppEvent_Header*), void* p_closure)
{
	Callback l_c = { (void(*)(void*, void*)) p_callback , p_closure };
	Observer_Register(p_observer, &l_c);
};

PlatformSpecificEventHook EventPoolHook;

void AppEvent_initialize()
{
	Observer_Alloc(&EventDispatcher);
	Observer_Alloc(&EventPoolHook);
	initPlatformSpecificEventCallback();
};

void AppEvent_pool()
{
	poolEventPlatformSepcific();
};

void AppEvent_free()
{
	Observer_Free(&EventDispatcher); Observer_Free(&EventPoolHook);
};

#ifdef _WIN32


inline void AppEventObserver_Broadcast(AppEventObserver* p_observer, AppEvent_Header* p_appEvent)
{
	Observer_Broadcast(p_observer, p_appEvent);
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void poolEventPlatformSepcific()
{
	MSG msg = {0};
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
	WNDCLASS wc = {0};
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
		WindowEvent l_windowEvent = { .Header = AppEventType_WINDOW_CLOSE, .Window = hwnd };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_windowEvent);
		return 0;
	}
	case WM_SIZE:
	{
		INT l_width = LOWORD(lParam);
		INT l_height = HIWORD(lParam);

		WindowResizeEvent l_windowResizeEvent = { .Header = AppEventType_WINDOW_RESIZE, .Window = hwnd, .Width = l_width, .Height = l_height };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_windowResizeEvent);
		return 0;
	}
	case WM_PAINT:
	{
		AppEvent_Header l_paintEvent = AppEventType_WINDOW_PAINT;
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_paintEvent);
		return 0;
	}
	case WM_KEYDOWN:
	{
		InputKeyEvent l_inputKeyEvent = { .Header = AppEventType_INPUT_KEY_EVENT, .Key = (int)wParam , .Type = InputKeyEventType_DOWN };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
		return 0;
	}
	case WM_KEYUP:
	{
		InputKeyEvent l_inputKeyEvent = { .Header = AppEventType_INPUT_KEY_EVENT, .Key = (int)wParam , .Type = InputKeyEventType_UP };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		InputKeyEvent l_inputKeyEvent = { .Header = AppEventType_INPUT_KEY_EVENT, .Key = WM_LBUTTONDOWN , .Type = InputKeyEventType_DOWN };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
		return 0;
	}
	break;
	case WM_LBUTTONUP:
	{
		InputKeyEvent l_inputKeyEvent = { .Header = AppEventType_INPUT_KEY_EVENT, .Key = WM_LBUTTONUP , .Type = InputKeyEventType_UP };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputKeyEvent);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);

		InputMouseEvent l_inputMouseEvent = { .Header = AppEventType_INPUT_MOUSE_EVENT, .X = xPos, .Y = yPos };
		AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_inputMouseEvent);
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
	AppEventObserver_Broadcast(&EventDispatcher, (AppEvent_Header*)&l_genericEvent);
};

#endif
