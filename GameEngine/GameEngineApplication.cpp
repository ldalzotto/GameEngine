#include "GameEngineApplication.h"

namespace _GameEngine
{
    GameEngineApplication *app_alloc()
    {
        return new GameEngineApplication();
    }

    void app_init(GameEngineApplication *p_app)
    {
        p_app->Log = Log_alloc();
        p_app->Render = Render_alloc();
    };

    void app_free(GameEngineApplication *p_app)
    {
        Log_free(p_app->Log);
        Render_free(p_app->Render);
        delete p_app;
    }

    void app_mainLoop(GameEngineApplication *p_app)
    {
        while (!Window_askedForClose(&p_app->Render->Window))
        {
            glfwPollEvents();
        }
    };

    void app_cleanup(GameEngineApplication *p_app)
    {
        Window_closeWindow(&p_app->Render->Window);
    };

    void app_run(GameEngineApplication *p_app)
    {
        app_init(p_app);
        app_mainLoop(p_app);
        app_cleanup(p_app);
    };
} // namespace _GameEngine