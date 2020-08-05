#include "Surface.h"
#include "RenderInterface.h"

#include "AppEvent/AppEvent.hpp"
#include "Window.h"

#include <stdexcept>
#include "Log/LogFormatting.hpp"

namespace _GameEngine::_Render
{
	VkResult surfePlatformSpecific_create(Surface* p_surface, RenderInterface* p_renderInterface);

	void Surface_build(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		if (surfePlatformSpecific_create(p_surface, p_renderInterface) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create window surface!"));
		}
	};

	void Surface_release(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		vkDestroySurfaceKHR(*p_renderInterface->Instance, p_surface->WindowSurface, nullptr);
	};

}


#ifdef _WIN32
namespace _GameEngine::_Render
{
	VkResult surfePlatformSpecific_create(Surface* p_surface, RenderInterface* p_renderInterface)
	{
		VkWin32SurfaceCreateInfoKHR l_surfaceCreateInfo{};
		l_surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		l_surfaceCreateInfo.hinstance = _Core::GlobalAppParams.hInstance;
		l_surfaceCreateInfo.hwnd = p_renderInterface->Window->Handle.Window;

		return vkCreateWin32SurfaceKHR(*p_renderInterface->Instance, &l_surfaceCreateInfo, NULL, &p_surface->WindowSurface);
	};
}

#endif