#pragma once

#include "Objects/Window/Window.hpp"

namespace _RenderV2
{
	struct RenderV2
	{
		Window AppWindow;
	};

	void RenderV2_initialize(RenderV2* p_render);
	void RenderV2_free(RenderV2* p_render);
}