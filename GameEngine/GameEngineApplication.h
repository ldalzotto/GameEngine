#pragma once

#include "Log/Log.h"
#include "Render/Render.h"
#include "GameLoop/GameLoop.h"

namespace _GameEngine
{

    struct GameEngineApplication
    {
        GameLoop *GameLoop;
        _Render::Render *Render;
    };

    GameEngineApplication *app_alloc();
    void app_free(GameEngineApplication *p_app);
    void app_init(GameEngineApplication *p_app);
    void app_mainLoop(GameEngineApplication *p_app);
    void app_cleanup(GameEngineApplication *p_app);
    void app_run(GameEngineApplication *p_app);
} // namespace _GameEngine
