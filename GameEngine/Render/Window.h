#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

namespace _GameEngine
{
	struct Window
	{
		GLFWwindow* Window;
	};

	void Window_init(Window* p_window);
	void Window_closeWindow(Window* p_window);
	bool Window_askedForClose(Window* p_window);
	void Window_get_instanceExtension(char*** out_extension, uint32_t* out_extensionCount);
	std::vector<std::string> Window_getRequiredExtensions();
} // namespace _GameEngine