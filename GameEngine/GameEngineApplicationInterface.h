#pragma once

#include <functional>

typedef struct Clock_TYP Clock;

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
}

namespace _Core
{
	template <typename INPUT_TYPE = _GameEngine::GameEngineApplicationInterface>
	struct ObserverT;
}

namespace _GameEngine
{
	struct GameEngineApplication;
	struct GameLoop;
	struct UpdateSequencer;
	namespace _Input { struct Input; }
	namespace _ECS { struct ECS; }
	namespace _Physics { struct PhysicsInterface; }
}

typedef struct RenderV2Interface_TYP RenderV2Interface;
typedef struct Log_TYP Log;

namespace _GameEngine
{
	struct GameEngineApplicationInterface
	{
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface>* NewFrame;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface>* PreRender;
		UpdateSequencer* UpdateSequencer;
		std::function<void(float)>* SandboxUpdateHook;

		GameLoop* GameLoop;
		Log* Log;
		Clock* Clock;
		_Physics::PhysicsInterface* PhysicsInterface;
		RenderV2Interface* RenderInterface;
		_Input::Input* Input;
		// _ECS::ECS* ECS;
	};

	void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication);
}