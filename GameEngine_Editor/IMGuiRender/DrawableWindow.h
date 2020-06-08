#pragma once

#include <vector>

namespace _GameEngineEditor
{
	struct DrawableWindow
	{
		bool IsOpen;
		void* Window;
		void(*Draw)(DrawableWindow* p_drawableWIndow);
		void(*OnClosed)(DrawableWindow* p_drawableWIndow);
	};

	void DrawableWindow_draw(DrawableWindow* p_drawableWindow);


	struct DrawableWindows
	{
		std::vector<DrawableWindow> DrawableWindows;
	};

	void DrawableWindows_draw(DrawableWindows* p_drawableWindow);
}