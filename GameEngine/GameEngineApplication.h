#pragma once

#include "GameEngineApplicationConfiguration.hpp"
#include <functional>

#include "Functional/Callback/ObserverT.hpp"

extern "C"
{
#include "Clock/Clock.h"
#include "RenderV2.h"
}
#include "Log/Log.hpp"

#include "GameLoop/GameLoop.h"
#include "EngineSequencers/EngineSequencers.h"

#include "Physics/Physics.h"
#include "ECS/ECS.h"
#include "Input/Input.h"

#if GAMEENGINE_EDITOR
#include "Editor/GameEngineEditor.h"
#endif

#include "GameEngineApplicationInterface.h"

namespace _GameEngine
{

	struct GameEngineApplication
	{
		GameEngineApplicationInterface GameEngineApplicationInterface;

		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> NewFrame;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> PreRender;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> EndOfUpdate;
		UpdateSequencer UpdateSequencer;

		GameLoop GameLoop;
		::_Core::Log Log;
		Clock Clock;
		_Physics::Physics Physics;
		RenderV2 Render;
		_Input::Input Input;
		_ECS::ECS ECS;

#if GAMEENGINE_EDITOR
		_GameEngineEditor::GameEngineEditor Editor;
#endif
	};

	GameEngineApplication* app_alloc();
	void app_free(GameEngineApplication* p_app);
	void app_mainLoop(GameEngineApplication* p_app);
	int app_run(GameEngineApplication* p_app);
} // namespace _GameEngine
