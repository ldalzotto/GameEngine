#include "Window.h"

#include <math.h>
#include "AppEvent/AppEvent.h"
#include "Objects/Texture/Texture.h"
#include "Functional/Callback/Observer.h"


void windowHookToGlobalEvents(Window* p_window);
void windowPlatforwSpecific_open(Window* p_window);
void windowPlatforwSpecific_close(Window* p_window);
void windowPlatformSpecific_paintTexture(Window* p_window);
void windowPlatforwSpecific_getClientRect(Window* p_windout, int* out_width, int* out_height);

const uint32_t WINDOW_WIDTH = 320;
const uint32_t WINDOW_HEIGHT = 240;

const char* WINDOW_ERROR_NOT_INITIALIZED = "The Window->Window is not initialized.";

void WindowSize_UpdateDependentValues(WindowSize* p_windowSize)
{
	p_windowSize->HalfWidth = p_windowSize->Width / 2;
	p_windowSize->HalfHeight = p_windowSize->Height / 2;
	p_windowSize->TwoOnHeight = 2.0f / p_windowSize->Height;
	p_windowSize->TwoOnWidth = 2.0f / p_windowSize->Width;
}

void Window_init(Window* p_window)
{
	Observer_Alloc(&p_window->OnWindowSizeChanged);

	p_window->WindowSize.Width = WINDOW_WIDTH;
	p_window->WindowSize.Height = WINDOW_HEIGHT;
	WindowSize_UpdateDependentValues(&p_window->WindowSize);

	windowHookToGlobalEvents(p_window);
	windowPlatforwSpecific_open(p_window);
}

void Window_closeWindow(Window* p_window)
{
	Observer_Free(&p_window->OnWindowSizeChanged);
	windowPlatforwSpecific_close(p_window);
	p_window->Handle.Window = NULL;
};

bool Window_askedForClose(Window* p_window)
{
	return p_window->WindowState.AskedForClosed;
};

WindowSize Window_getSize(Window* p_window)
{
	return p_window->WindowSize;
};

bool Window_consumeSizeChangeEvent(Window* p_window)
{
	if (p_window->WindowState.HasResizedThisFrame)
	{
		windowPlatforwSpecific_getClientRect(p_window, (int*)&p_window->WindowSize.Width, (int*)&p_window->WindowSize.Height);
		WindowSize_UpdateDependentValues(&p_window->WindowSize);
		
		Observer_Broadcast(&p_window->OnWindowSizeChanged, NULL);

		p_window->WindowState.HasResizedThisFrame = false;
		return true;
	}
	return false;
};

void window_onGlobalEvent(Window* p_window, AppEvent_Header* p_eventHeader)
{
	switch (*p_eventHeader)
	{
	case AppEventType_WINDOW_CLOSE:
	{
		WindowEvent* l_windowEvent = (WindowEvent*)p_eventHeader;
		if (memcmp(&l_windowEvent->Window, &p_window->Handle.Window, sizeof(WindowHandle)) == 0)
		{
			p_window->WindowState.AskedForClosed = true;
		}
	}
	break;
	case AppEventType_WINDOW_RESIZE:
	{
		WindowResizeEvent* l_windowResizeEvent = (WindowResizeEvent*)p_eventHeader;
		if (memcmp(&l_windowResizeEvent->Window, &p_window->Handle.Window, sizeof(WindowHandle)) == 0)
		{
			p_window->WindowState.HasResizedThisFrame = true;
			p_window->WindowState.HasResizedThisFrame_WindowSize = (WindowSize){ (uint32_t)l_windowResizeEvent->Width , (uint32_t)l_windowResizeEvent->Height };
		}
	}
	break;
	case AppEventType_WINDOW_PAINT:
	{
		windowPlatformSpecific_paintTexture(p_window);
	}
	break;
	}
}

void windowHookToGlobalEvents(Window* p_window)
{
	AppEventObserver_Register(&EventDispatcher, (void(*)(void*, AppEvent_Header*))window_onGlobalEvent, p_window);
};

void Window_presentTexture(Window* p_window, Texture3c_PTR p_texture)
{
	p_window->WindowState.PendingPresentingTexture = p_texture;

	InvalidateRect(p_window->Handle.Window, NULL, TRUE);
	SendMessage(p_window->Handle.Window, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);
};

#ifdef  _WIN32


void windowPlatforwSpecific_open(Window* p_window)
{
	DWORD l_windowStyle = WS_OVERLAPPEDWINDOW;

	RECT l_clientRect = { 0 };
	l_clientRect.right = p_window->WindowSize.Width;
	l_clientRect.bottom = p_window->WindowSize.Height;
	AdjustWindowRect(&l_clientRect, l_windowStyle, false);;

	int l_windowWidth = l_clientRect.right - l_clientRect.left;
	int l_windowHeight = l_clientRect.bottom - l_clientRect.top;

	p_window->Handle.Window = CreateWindowEx(0, "GameEngine", "Learn to Program Windows", l_windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, l_windowWidth, l_windowHeight,
		NULL, NULL, GlobalAppParams.hInstance, NULL);
	ShowWindow(p_window->Handle.Window, SW_SHOW);
};

void windowPlatforwSpecific_close(Window* p_window)
{
	DestroyWindow(p_window->Handle.Window);
};

void windowPlatformSpecific_paintTexture(Window* p_window)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(p_window->Handle.Window, &ps);

	if (p_window->WindowState.PendingPresentingTexture)
	{
		Texture3c_PTR l_presentTexture = p_window->WindowState.PendingPresentingTexture;
		//	p_window->WindowState.PendingPresentingTexture = false;

		HDC l_map_hdc = CreateCompatibleDC(hdc);
		HBITMAP l_map = CreateCompatibleBitmap(hdc, l_presentTexture->Width, l_presentTexture->Height);
		SelectObject(l_map_hdc, l_map);

		{
			BITMAP structBitmapHeader;
			memset(&structBitmapHeader, 0, sizeof(BITMAP));
			HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
			GetObject(hBitmap, sizeof(BITMAP), &structBitmapHeader);
		}

		//Set bitmap header
		{
			BITMAPINFOHEADER bmih;
			bmih.biSize = sizeof(BITMAPINFOHEADER);
			bmih.biBitCount = 8 * (3 * sizeof(char));
			bmih.biClrImportant = 0;
			bmih.biClrUsed = 0;
			bmih.biCompression = BI_RGB;
			bmih.biHeight = -l_presentTexture->Height;
			bmih.biWidth = l_presentTexture->Width;
			bmih.biPlanes = 1;
			bmih.biSizeImage = (DWORD)Texture_GetSizeInBytes_3C(l_presentTexture);
			bmih.biXPelsPerMeter = 0;
			bmih.biYPelsPerMeter = 0;

			BITMAPINFO bmpInfo;
			bmpInfo.bmiHeader = bmih;
			bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);

			// SetDIBitsToDevice(l_map_hdc, )

			if (!SetDIBits(l_map_hdc,
				l_map,
				0,
				l_presentTexture->Height,
				l_presentTexture->Pixels.Memory,
				&bmpInfo,
				DIB_RGB_COLORS))
			{
				printf("windowPlatformSpecific_paintTexture : cannot set blit texture !");
				abort();
			};
		}


		{
			if (!BitBlt(hdc, 0, 0, l_presentTexture->Width, l_presentTexture->Height, l_map_hdc, 0, 0, SRCCOPY))
			{
				printf("windowPlatformSpecific_paintTexture : cannot blit texture !");
				abort();
			};
		}



		DeleteDC(l_map_hdc);
		DeleteObject(l_map);
		ReleaseDC(p_window->Handle.Window, hdc);
	}

	EndPaint(p_window->Handle.Window, &ps);


};

void windowPlatforwSpecific_getClientRect(Window* p_windout, int* out_width, int* out_height)
{
	RECT l_clientRect;
	GetClientRect(p_windout->Handle.Window, &l_clientRect);

	*out_width = l_clientRect.right - l_clientRect.left;
	*out_height = l_clientRect.bottom - l_clientRect.top;
};

#endif //  _WIN32

