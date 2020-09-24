#pragma once

#include "Functional/Callback/Observer_def.h"
#include "v2/_interface/WindowSize_def.h"
#include "Objects/Texture/Texture_def.h"

#ifdef _WIN32
#include "Include/PlatformInclude.h"

typedef struct WindowHandle_TYP
{
	HWND Window;
} WindowHandle;

#endif

typedef struct WindowState_TYP
{
	bool AskedForClosed;
	bool HasResizedThisFrame;
	WindowSize HasResizedThisFrame_WindowSize;
	Texture3c_PTR PendingPresentingTexture;
} WindowState;

typedef Observer OnWindowSizeChangedObserver;

typedef struct Window_TYP
{
	WindowHandle Handle;
	WindowSize WindowSize;

	WindowState WindowState;

	OnWindowSizeChangedObserver OnWindowSizeChanged;
}Window, * Window_PTR;