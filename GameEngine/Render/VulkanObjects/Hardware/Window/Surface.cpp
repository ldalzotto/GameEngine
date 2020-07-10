#include "Surface.h"
#include "RenderInterface.h"

#include "Window.h"

#include <stdexcept>
extern "C"
{
#include "Log/LogFormatting.h"
}
#include "Includes/GLFWIncludes.h"

namespace _GameEngine::_Render
{
	void Surface_build(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		if (glfwCreateWindowSurface(*p_renderInterface->Instance, p_renderInterface->Window->Window, nullptr, &p_surface->WindowSurface) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create window surface!"));
		}
	};

	void Surface_release(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		vkDestroySurfaceKHR(*p_renderInterface->Instance, p_surface->WindowSurface, nullptr);
	};

}


