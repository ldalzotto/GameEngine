#include "Window.h"

#include <unordered_map>
#include <stdexcept>

#include "Log/LogFormatting.hpp"
#include "v2/Matrix/MatrixMath.hpp"

namespace _GameEngine::_Render
{
	const uint32_t WINDOW_WIDTH = 800;
	const uint32_t WINDOW_HEIGHT = 600;

	const char* WINDOW_ERROR_NOT_INITIALIZED = "The Window->Window is not initialized.";

	// void window_size_callback(GLFWwindow* window, int width, int height);
	void Window_updateScreeToGraphicsAPIPixelCoordinates(Window* p_window)
	{
		_MathV2::Vector3<float> l_right, l_up, l_forward;
		l_right = { 2.0f / (float)p_window->WindowSize.Width , 0.0f, 0.0f };
		l_up = { 0.0f, 2.0f / (float)p_window->WindowSize.Height, 0.0f };
		l_forward = { -1.0f, -1.0f, 0.0f };
		p_window->WindowToGraphicsAPIPixelCoordinates = _MathV2::MatrixM::build(&l_right, &l_up, &l_forward);
	};

	// std::unordered_map<GLFWwindow*, Window*> WindowIndexedForGLFW{};

	void Window_init(Window* p_window)
	{
		_Core::ObserverT_alloc(&p_window->OnWindowSizeChanged);

		/*
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		p_window->Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
		p_window->WindowSize.Width = WINDOW_WIDTH;
		p_window->WindowSize.Height = WINDOW_HEIGHT;
		Window_updateScreeToGraphicsAPIPixelCoordinates(p_window);
		WindowIndexedForGLFW.emplace(p_window->Window, p_window);
		glfwSetWindowSizeCallback(p_window->Window, window_size_callback);
		*/
	}

	void Window_closeWindow(Window* p_window)
	{
		_Core::ObserverT_free(&p_window->OnWindowSizeChanged);

		/*
		glfwDestroyWindow(p_window->Window);
		glfwTerminate();
		WindowIndexedForGLFW.erase(p_window->Window);
		p_window->Window = nullptr;
		*/
	};

	bool Window_askedForClose(Window* p_window)
	{
		return false;
		// return glfwWindowShouldClose(p_window->Window);
	};

	WindowSize Window_getSize(Window* p_window)
	{
		return p_window->WindowSize;
	};

	std::vector<char*> Window_getRequiredExtensionsV2(Window* p_window)
	{

		/*
#ifndef NDEBUG
		if (!p_window->Window)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE((char*)WINDOW_ERROR_NOT_INITIALIZED));
		}
#endif

		uint32_t l_extensionCount;
		char** l_extensions = (char**)glfwGetRequiredInstanceExtensions(&l_extensionCount);

		std::vector<char*> l_requiredExtensions(l_extensionCount);

		for (uint32_t i = 0; i < l_extensionCount; i++)
		{
			l_requiredExtensions.at(i) = l_extensions[i];
		}
		*/
		return std::vector<char*>();
	};

	/*
	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Window* l_window = WindowIndexedForGLFW.at(window);

		l_window->WindowSize.Width = width;
		l_window->WindowSize.Height = height;
		Window_updateScreeToGraphicsAPIPixelCoordinates(l_window);
		_Core::ObserverT_broadcast(&l_window->OnWindowSizeChanged, (void*)nullptr);
	};
	*/

} // namespace _GameEngine