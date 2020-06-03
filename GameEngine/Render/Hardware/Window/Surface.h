#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct Surface
	{
		VkSurfaceKHR WindowSurface;
	};

	void Surface_build(Surface* p_surface, RenderInterface* p_renderInterface);
	void Surface_release(Surface* p_surface, RenderInterface* p_renderInterface);
}