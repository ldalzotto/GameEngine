#include "GameEngineApplication.h"

namespace _GameEngine
{
    GameEngineApplication *app_alloc()
    {
        GameEngineApplication *l_gameEngineApplication = new GameEngineApplication();
        l_gameEngineApplication->Log = Log_alloc();
        return l_gameEngineApplication;
    }

    void app_free(GameEngineApplication *p_app)
    {
        Log_free(p_app->Log);
        delete p_app;
    }

    void app_initWindow(GameEngineApplication *p_app)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        p_app->Window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    };

    void app_initVulkan(GameEngineApplication *p_app){

    };

    void app_mainLoop(GameEngineApplication *p_app)
    {
        while (!glfwWindowShouldClose(p_app->Window))
        {
            p_app->Log->CoreLogger->info("HelloWorld");
            glfwPollEvents();
        }
    };

    void app_cleanup(GameEngineApplication *p_app)
    {
        glfwDestroyWindow(p_app->Window);
        glfwTerminate();
    };

    void app_run(GameEngineApplication *p_app)
    {
        app_initWindow(p_app);
        app_initVulkan(p_app);
        app_mainLoop(p_app);
        app_cleanup(p_app);
    };
} // namespace _GameEngine