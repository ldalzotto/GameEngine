#include "Window.h"

namespace _GameEngine
{
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
	};

	bool Window_askedForClose(Window* p_window)
	{
		return glfwWindowShouldClose(p_window->Window);
	};

	void Window_get_instanceExtension(char*** out_extension, uint32_t* out_extensionCount)
	{
		*out_extension = (char**)glfwGetRequiredInstanceExtensions(out_extensionCount);
	};

	std::vector<std::string> Window_getRequiredExtensions()
	{
		uint32_t l_extensionCount;
		const char** l_extensions = glfwGetRequiredInstanceExtensions(&l_extensionCount);

		std::vector<std::string> l_requiredExtensions(l_extensionCount);
		
		for (int i = 0; i < l_extensionCount; i++) 
		{
			l_requiredExtensions.at(i) = std::string(l_extensions[i]);
		}

		return l_requiredExtensions;
	};
} // namespace _GameEngine