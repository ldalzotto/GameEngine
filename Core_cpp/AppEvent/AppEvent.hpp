#pragma once

#include "Functional/Callback/ObserverT.hpp"

namespace _Core
{
	enum AppEventType 
	{
		WINDOW_CLOSE,
		WINDOW_RESIZE
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

	struct AppEventParams;

	extern AppEventParams GlobalAppParams;
	extern ObserverT<AppEvent_Header> EventDispatcher;

	void AppEvent_initialize(AppEventParams* p_params);
	bool AppEvent_pool();
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

