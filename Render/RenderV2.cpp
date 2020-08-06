#include "RenderV2.hpp"

namespace _RenderV2
{
	void RenderV2_initialize(RenderV2* p_render)
	{
		Window_init(&p_render->AppWindow);
	}

	void RenderV2_free(RenderV2* p_render)
	{
	}
}