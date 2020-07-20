#pragma once

#include <functional>

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
}

namespace _Core
{
	struct Log;
	struct Clock;

	template <typename INPUT_TYPE = _GameEngine::GameEngineApplicationInterface>
	struct ObserverT;
}

namespace _GameEngine
{
	struct GameEngineApplication;
	struct GameLoop;
	struct UpdateSequencer;
	namespace _Render { struct RenderInterface; }
	namespace _Input { struct Input; }
	namespace _ECS { struct ECS; }
	namespace _Physics { struct PhysicsInterface; }
}

namespace _GameEngine
{
	struct GameEngineApplicationInterface
	{
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface>* NewFrame;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface>* PreRender;
		UpdateSequencer* UpdateSequencer;
		std::function<void(float)>* SandboxUpdateHook;

		GameLoop* GameLoop;
		::_Core::Log* Log;
		::_Core::Clock* Clock;
		_Physics::PhysicsInterface* PhysicsInterface;
		_Render::RenderInterface* RenderInterface;
		_Input::Input* Input;
		_ECS::ECS* ECS;
	};

	void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication);
}