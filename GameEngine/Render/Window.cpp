#include "Window.h"

namespace _GameEngine
{
    void Window_init(Window *p_window)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        p_window->Window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    }

    void Window_closeWindow(Window *p_window)
    {
        glfwDestroyWindow(p_window->Window);
        glfwTerminate();
    };

    bool Window_askedForClose(Window *p_window)
    {
        return glfwWindowShouldClose(p_window->Window);
    };
} // namespace _GameEngine