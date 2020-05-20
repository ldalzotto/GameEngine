#include "Window.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render::_Window
{

	const std::string WINDOW_ERROR_NOT_INITIALIZED = "The Window->Window is not initialized.";

	void Window_init(Window* p_window)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		p_window->Window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	}

	void Window_closeWindow(Window* p_window)
	{
		glfwDestroyWindow(p_window->Window);
		glfwTerminate();
		p_window->Window = nullptr;
	};

	bool Window_askedForClose(Window* p_window)
	{
		return glfwWindowShouldClose(p_window->Window);
	};

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window)
	{

#ifndef NDEBUG
		if (!p_window->Window)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE(WINDOW_ERROR_NOT_INITIALIZED));
		}
#endif
		
		uint32_t l_extensionCount;
		char** l_extensions = (char**) glfwGetRequiredInstanceExtensions(&l_extensionCount);

		std::vector<char*> l_requiredExtensions(l_extensionCount);

		for (uint32_t i = 0; i < l_extensionCount; i++)
		{
			l_requiredExtensions.at(i) = l_extensions[i];
		}

		return l_requiredExtensions;
	};
} // namespace _GameEngine