#pragma once

#include "GameEngineApplicationConfiguration.hpp"
#include <functional>

#include "Functional/Callback/ObserverT.hpp"

extern "C"
{
#include "Clock/Clock.h"
#include "RenderV2.h"
#include "Log/Log_def.h"
#include "ECS_def.h"
#include "ECSEngine/Systems/CameraRenderSystem_def.h"
#include "ECSEngine/Systems/MeshDrawSystem_def.h"
}

#include "ECSEngine/Systems/PhysicsSystem.hpp"

#include "GameLoop/GameLoop.h"

#include "Physics/Physics.h"

#include "Input/Input.h"

#if GAMEENGINE_EDITOR
#include "Editor/GameEngineEditor.h"
#endif

#include "GameEngineApplicationInterface.h"

namespace _GameEngine
{

	struct GameEngineApplicationSystems
	{
		CameraRenderSystem CameraRenderSystem;
		MeshDrawSystem MeshDrawSystem;
		PhysicsSystem PhysicsSystem;
	};

	struct GameEngineApplication
	{
		GameEngineApplicationInterface GameEngineApplicationInterface;

		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> NewFrame;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> PreRender;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> EndOfUpdate;

		GameLoop GameLoop;
		Log Log;
		Clock Clock;
		_Physics::Physics Physics;
		RenderV2 Render;
		_Input::Input Input;
		ECS ECS;

		GameEngineApplicationSystems Systems;

#if GAMEENGINE_EDITOR
		_GameEngineEditor::GameEngineEditor Editor;
#endif
	};

	GameEngineApplication* app_alloc();
	void app_free(GameEngineApplication* p_app);
	void app_mainLoop(GameEngineApplication* p_app);
	int app_run(GameEngineApplication* p_app);
} // namespace _GameEngine
