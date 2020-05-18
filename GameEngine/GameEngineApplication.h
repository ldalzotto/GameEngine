#pragma once

#include "Log/Log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace _GameEngine
{
    struct GameEngineApplication
    {
        GLFWwindow *Window;
        Log *Log;
    };

    GameEngineApplication *app_alloc();
    void app_free(GameEngineApplication *p_app);
    void app_initWindow(GameEngineApplication *p_app);
    void app_initVulkan(GameEngineApplication *p_app);
    void app_mainLoop(GameEngineApplication *p_app);
    void app_cleanup(GameEngineApplication *p_app);
    void app_run(GameEngineApplication *p_app);
} // namespace _GameEngine
