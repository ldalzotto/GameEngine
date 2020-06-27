#include "Window.h"

#include <unordered_map>
#include <stdexcept>
#include "MYLog/MYLog.h"

namespace _GameEngine::_Render
{
	const uint32_t WINDOW_WIDTH = 800;
	const uint32_t WINDOW_HEIGHT = 600;

	const std::string WINDOW_ERROR_NOT_INITIALIZED = "The Window->Window is not initialized.";

	void window_size_callback(GLFWwindow* window, int width, int height);
	void Window_updateScreeToGraphicsAPIPixelCoordinates(Window* p_window)
	{
			p_window->WindowToGraphicsAPIPixelCoordinates._00 = 2.0f / (float)p_window->WindowSize.Width;
			p_window->WindowToGraphicsAPIPixelCoordinates._01 = 0.0f;
			p_window->WindowToGraphicsAPIPixelCoordinates._02 = 0.0f;

			p_window->WindowToGraphicsAPIPixelCoordinates._10 = 0.0f;
			p_window->WindowToGraphicsAPIPixelCoordinates._11 = 2.0f / (float)p_window->WindowSize.Height;
			p_window->WindowToGraphicsAPIPixelCoordinates._12 = 0.0f;

			p_window->WindowToGraphicsAPIPixelCoordinates._20 = -1.0f;
			p_window->WindowToGraphicsAPIPixelCoordinates._21 = -1.0f;
			p_window->WindowToGraphicsAPIPixelCoordinates._22 = 0.0f;
	};

	std::unordered_map<GLFWwindow*, Window*> WindowIndexedForGLFW{};

	void Window_init(Window* p_window)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		p_window->Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
		p_window->WindowSize.Width = WINDOW_WIDTH;
		p_window->WindowSize.Height = WINDOW_HEIGHT;
		Window_updateScreeToGraphicsAPIPixelCoordinates(p_window);
		WindowIndexedForGLFW.emplace(p_window->Window, p_window);
		glfwSetWindowSizeCallback(p_window->Window, window_size_callback);
	}

	void Window_closeWindow(Window* p_window)
	{
		glfwDestroyWindow(p_window->Window);
		glfwTerminate();
		WindowIndexedForGLFW.erase(p_window->Window);
		p_window->Window = nullptr;
	};

	bool Window_askedForClose(Window* p_window)
	{
		return glfwWindowShouldClose(p_window->Window);
	};

	WindowSize Window_getSize(Window* p_window)
	{
		return p_window->WindowSize;
	};

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window)
	{

#ifndef NDEBUG
		if (!p_window->Window)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE(WINDOW_ERROR_NOT_INITIALIZED));
		}
#endif

		uint32_t l_extensionCount;
		char** l_extensions = (char**)glfwGetRequiredInstanceExtensions(&l_extensionCount);

		std::vector<char*> l_requiredExtensions(l_extensionCount);

		for (uint32_t i = 0; i < l_extensionCount; i++)
		{
			l_requiredExtensions.at(i) = l_extensions[i];
		}

		return l_requiredExtensions;
	};

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Window* l_window = WindowIndexedForGLFW.at(window);

		l_window->WindowSize.Width = width;
		l_window->WindowSize.Height = height;
		Window_updateScreeToGraphicsAPIPixelCoordinates(l_window);
		Observer_broadcast(&l_window->OnWindowSizeChanged, nullptr);
	};

} // namespace _GameEngine