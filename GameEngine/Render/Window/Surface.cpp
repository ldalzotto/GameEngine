
#include <stdexcept>

#include "Log/Log.h"
#include "Surface.h"
#include "Render/Includes/GLFWIncludes.h"

namespace _GameEngine::_Render::_Surface
{
	void build(Surface* p_surface, VkInstance p_instance, _Window::Window* p_window)
	{
		if (glfwCreateWindowSurface(p_instance, p_window->Window, nullptr, &p_surface->WindowSurface) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create window surface!"));
		}
	};

	void release(Surface* p_surface, VkInstance p_instance)
	{
		vkDestroySurfaceKHR(p_instance, p_surface->WindowSurface, nullptr);
	};

}


