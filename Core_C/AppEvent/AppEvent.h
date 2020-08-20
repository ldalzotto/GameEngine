#pragma once


#include "Functional/Callback/Observer_def.h"

typedef char AppEventType;
#define AppEventType_WINDOW_CLOSE 0
#define AppEventType_WINDOW_RESIZE 1
#define AppEventType_WINDOW_PAINT 2
#define AppEventType_INPUT_KEY_EVENT 3
#define AppEventType_INPUT_MOUSE_EVENT 4

typedef AppEventType AppEvent_Header;

typedef struct WindowEvent_TYP
{
	AppEvent_Header Header;
	void* Window; //TOOD -> change pointer
} WindowEvent, * WindowEvent_PTR;

typedef struct WindowResizeEvent_TYP
{
	AppEvent_Header Header;
	void* Window; //TOOD -> change pointer
	int Width;
	int Height;
} WindowResizeEvent, * WindowResizeEvent_PTR;

typedef char InputKeyEventType;
#define InputKeyEventType_DOWN 0
#define InputKeyEventType_UP 1

typedef struct InputKeyEvent_TYP
{
	AppEvent_Header Header;
	int Key;
	InputKeyEventType Type;
}InputKeyEvent, * InputKeyEvent_PTR;

typedef struct InputMouseEvent_TYP
{
	AppEvent_Header Header;
	int X; int Y;
}InputMouseEvent, * InputMouseEvent_PTR;

typedef struct AppEventParams_TYP AppEventParams;

typedef Observer AppEventObserver;

void AppEventObserver_Register(AppEventObserver* p_observer, void(*p_callback)(void*, AppEvent_Header*), void* p_closure);

extern AppEventParams GlobalAppParams;
extern AppEventObserver EventDispatcher;

void AppEvent_initialize();
void AppEvent_pool();
void AppEvent_free();

typedef struct PlatformSpecificGenericEvent_TYP PlatformSpecificGenericEvent;
typedef Observer PlatformSpecificEventHook;
extern PlatformSpecificEventHook EventPoolHook;

#ifdef _WIN32

#include "Include/PlatformInclude.h"

typedef struct AppEventParams_TYP
{
	HINSTANCE hInstance;
}AppEventParams, * AppEventParams_PTR;

typedef struct PlatformSpecificGenericEvent_TYP
{
	HWND hwnd; UINT uMsg; WPARAM wParam; LPARAM lParam;
}PlatformSpecificGenericEvent, * PlatformSpecificGenericEvent_PTR;
#endif

