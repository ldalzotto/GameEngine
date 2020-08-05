#pragma once

#include "Functional/Callback/ObserverT.hpp"

namespace _Core
{
	enum AppEventType 
	{
		WINDOW_CLOSE
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

	extern ObserverT<AppEvent_Header> EventDispatcher;

	struct AppEventParams;

	void AppEvent_initialize(AppEventParams* p_params);
	bool AppEvent_pool();
	void AppEvent_free();
}

#ifdef _WIN32
#include <Windows.h>
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

