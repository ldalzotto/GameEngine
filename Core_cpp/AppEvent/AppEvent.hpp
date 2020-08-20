#pragma once


extern "C"
{
#include "Functional/Callback/Observer_def.h"
}

namespace _Core
{
	enum AppEventType 
	{
		WINDOW_CLOSE,
		WINDOW_RESIZE,
		WINDOW_PAINT,
		INPUT_KEY_EVENT,
		INPUT_MOUSE_EVENT
	};

	struct AppEvent_Header
	{
		AppEventType EventType;
	};

	struct WindowEvent
	{
		AppEvent_Header Header;
		void* Window; //TOOD -> change pointer
	};

	struct WindowResizeEvent
	{
		AppEvent_Header Header;
		void* Window; //TOOD -> change pointer
		int Width;
		int Height;
	};

	enum class InputKeyEventType
	{
		DOWN, UP 
	};

	struct InputKeyEvent
	{
		AppEvent_Header Header;
		int Key;
		InputKeyEventType Type;
	};

	struct InputMouseEvent
	{
		AppEvent_Header Header;
		int X; int Y;
	};

	struct AppEventParams;

	typedef Observer AppEventObserver;
	
	void AppEventObserver_Register(AppEventObserver* p_observer, void(*p_callback)(void*, AppEvent_Header*), void* p_closure);

	extern AppEventParams GlobalAppParams;
	extern AppEventObserver EventDispatcher;

	void AppEvent_initialize();
	void AppEvent_pool();
	void AppEvent_free();

	struct PlatformSpecificGenericEvent;
	typedef Observer PlatformSpecificEventHook;
	extern PlatformSpecificEventHook EventPoolHook;
}

#ifdef _WIN32

#include "Include/PlatformInclude.h"
namespace _Core
{
	struct AppEventParams
	{
		HINSTANCE hInstance; 
	};

	struct PlatformSpecificGenericEvent
	{
		HWND hwnd; UINT uMsg; WPARAM wParam; LPARAM lParam;
	};
}
#endif

