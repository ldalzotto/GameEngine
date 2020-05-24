#pragma once


#include "vulkan/vulkan.h"

#include "Window.h"

namespace _GameEngine::_Render
{
	struct Surface
	{
		VkSurfaceKHR WindowSurface;
	};

	void Surface_build(Surface* p_surface, VkInstance p_instance, Window* p_window);
	void Surface_release(Surface* p_surface, VkInstance p_instance);
}