#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace _GameEngine
{
    struct Window
    {
        GLFWwindow *Window;
    };

    void Window_init(Window *p_window);
    void Window_closeWindow(Window *p_window);
    bool Window_askedForClose(Window *p_window);
} // namespace _GameEngine