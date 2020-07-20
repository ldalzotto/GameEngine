#include "DrawableWindow.h"

namespace _GameEngineEditor
{
	void DrawableWindow_draw(DrawableWindow* p_drawableWindow)
	{
		if (p_drawableWindow->Draw)
		{
			p_drawableWindow->Draw(p_drawableWindow);
		}
	};

	void DrawableWindows_draw(DrawableWindows* p_drawableWindows)
	{
		size_t l_size = p_drawableWindows->DrawableWindows.size();
		for (size_t i = l_size - 1; i < l_size; --i)
		{
			DrawableWindow* l_window = &p_drawableWindows->DrawableWindows.at(i);
			if (!l_window->IsOpen)
			{
				if (l_window->OnClosed) { l_window->OnClosed(l_window); }
				p_drawableWindows->DrawableWindows.erase(p_drawableWindows->DrawableWindows.begin() + i);
			}
		}

		l_size = p_drawableWindows->DrawableWindows.size();
		for (size_t i = 0; i < l_size; i++)
		{
			DrawableWindow_draw(&p_drawableWindows->DrawableWindows.at(i));
		}
	};
}