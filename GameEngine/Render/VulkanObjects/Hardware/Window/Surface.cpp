#include "Surface.h"
#include "Render/RenderInterface.h"

#include "Window.h"

#include <stdexcept>
#include "Log/Log.h"
#include "Render/Includes/GLFWIncludes.h"

namespace _GameEngine::_Render
{
	void Surface_build(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		if (glfwCreateWindowSurface(*p_renderInterface->Instance, p_renderInterface->Window->Window, nullptr, &p_surface->WindowSurface) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create window surface!"));
		}
	};

	void Surface_release(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		vkDestroySurfaceKHR(*p_renderInterface->Instance, p_surface->WindowSurface, nullptr);
	};

}


