#pragma once

#include "Functional/Callback/ObserverT.hpp"

namespace _Core
{
	enum AppEventType 
	{
		WINDOW_CLOSE,
		WINDOW_RESIZE,
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

	extern AppEventParams GlobalAppParams;
	extern ObserverT<AppEvent_Header> EventDispatcher;

	void AppEvent_initialize(AppEventParams* p_params);
	void AppEvent_pool();
	void AppEvent_free();
}

#ifdef _WIN32

#include "Include/PlatformInclude.hpp"
namespace _Core
{
	struct AppEventParams
	{
		HINSTANCE hInstance; 
		HINSTANCE hPrevInstance;
		PWSTR pCmdLine;
		int nCmdShow;
	};
}
#endif

