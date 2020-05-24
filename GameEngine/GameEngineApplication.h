#pragma once

#include <functional>

#include "Log/Log.h"
#include "Render/Render.h"
#include "GameLoop/GameLoop.h"
#include "ECS/EntityComponent.h"

namespace _GameEngine
{

	struct GameEngineApplication
	{
		std::function<void(float)> SandboxUpdateHook;
		GameLoop* GameLoop;
		_Render::Render* Render;
		_ECS::EntityComponent* EntityComponent;
	};

	GameEngineApplication* app_alloc(const std::function<void(float)>& p_sandboxUpdateHook);
	void app_free(GameEngineApplication* p_app);
	void app_init(GameEngineApplication* p_app);
	void app_mainLoop(GameEngineApplication* p_app);
	void app_cleanup(GameEngineApplication* p_app);
	void app_run(GameEngineApplication* p_app);
} // namespace _GameEngine
