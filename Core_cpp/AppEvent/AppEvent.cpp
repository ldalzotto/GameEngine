#include "AppEvent.hpp"

namespace _Core
{
	void initPlatformSpecificEventCallback(AppEventParams* p_params);
	bool poolEventPlatformSepcific();

	ObserverT<AppEvent_Header> EventDispatcher{};

	void AppEvent_initialize(AppEventParams* p_params)
	{
		ObserverT_alloc(&EventDispatcher);
		initPlatformSpecificEventCallback(p_params);
	};

	bool AppEvent_pool()
	{
		return poolEventPlatformSepcific();
	};

	void AppEvent_free()
	{
		ObserverT_free(&EventDispatcher);
	};
}

#ifdef _WIN32

#include <Windows.h>

namespace _Core
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool poolEventPlatformSepcific()
	{
		MSG msg{};
		bool l_return = GetMessage(&msg, NULL, 0, 0);
		if (l_return)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return l_return;
	};

	void initPlatformSpecificEventCallback(AppEventParams* p_params)
	{
		LPCSTR CLASS_NAME = "GameEngine";
		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = p_params->hInstance;
		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);
	};

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			return 0;
		case WM_CLOSE:
		{
			WindowEvent l_windowEvent{ {AppEventType::WINDOW_CLOSE}, hwnd };
			ObserverT_broadcast(&EventDispatcher, (AppEvent_Header*)&l_windowEvent);
			DestroyWindow(hwnd);
		}
		return 0;
		case WM_DESTROY:
		{

		}
		return 0;
		case WM_LBUTTONDOWN:
		{
			int l_test = 0;
		}
		return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};
}

#endif
