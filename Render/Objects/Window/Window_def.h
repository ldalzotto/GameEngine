#pragma once

#include "Functional/Callback/Observer_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "Objects/Texture/Texture_def.h"

#ifdef _WIN32
#include "Include/PlatformInclude.h"

typedef struct WindowHandle_TYP
{
	HWND Window;
} WindowHandle;

#endif

typedef struct WindowSize_TYP
{
	uint32_t Width;
	uint32_t Height;
} WindowSize, * WindowSize_PTR;

typedef struct WindowState_TYP
{
	bool AskedForClosed;
	bool HasResizedThisFrame;
	WindowSize HasResizedThisFrame_WindowSize;
	TEXTURE3C_PTR PendingPresentingTexture;
} WindowState;

typedef Observer OnWindowSizeChangedObserver;

typedef struct Window_TYP
{
	WindowHandle Handle;
	WindowSize WindowSize;

	WindowState WindowState;

	MATRIX3F WindowToGraphicsAPIPixelCoordinates;
	MATRIX4F GraphicsAPIToWindowPixelCoordinates;

	OnWindowSizeChangedObserver OnWindowSizeChanged;
}Window, * Window_PTR;