#pragma once

#include <functional>

#include "GameLoop/GameLoop.h"
#include "EngineSequencers/EngineSequencers.h"
#include "Utils/Observer/Observer.h"

#include "Render.h"
#include "MyLog/MyLog.h"
#include "Clock/Clock.h"
#include "ECS/ECS.h"
#include "Input/Input.h"

#include "GameEngineApplicationInterface.h"

namespace _GameEngine
{
	struct GameEngineApplication
	{
		GameEngineApplicationInterface GameEngineApplicationInterface;

		_Utils::Observer NewFrame;
		_Utils::Observer PreRender;
		UpdateSequencer UpdateSequencer;
		std::function<void(float)> SandboxUpdateHook;

		GameLoop GameLoop;
		_Log::MyLog Log;
		_Clock::Clock Clock;
		_Render::Render Render;
		_Input::Input Input;
		_ECS::ECS ECS;
	};

	GameEngineApplication* app_alloc(const std::function<void(float)>& p_sandboxUpdateHook);
	void app_free(GameEngineApplication* p_app);
	void app_mainLoop(GameEngineApplication* p_app);
	void app_cleanup(GameEngineApplication* p_app);
	void app_run(GameEngineApplication* p_app);
} // namespace _GameEngine
