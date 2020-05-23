#pragma once

#include "vulkan/vulkan.h"

#include "Window.h"

namespace _GameEngine::_Render::_Hardware::_Surface
{
	struct Surface
	{
		VkSurfaceKHR WindowSurface;
	};

	void build(Surface* p_surface, VkInstance p_instance, _Window::Window* p_window);
	void release(Surface* p_surface, VkInstance p_instance);
}