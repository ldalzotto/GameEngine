#pragma once

#include <functional>

struct Core_Clock;

namespace _GameEngine
{
	struct GameEngineApplication;
	struct GameLoop;
	struct UpdateSequencer;
	namespace _Log { struct MyLog; }
	namespace _Render { struct RenderInterface; }
	namespace _Input { struct Input; }
	namespace _ECS { struct ECS; }
	namespace _Utils { struct Observer; }
}

namespace _GameEngine
{
	struct GameEngineApplicationInterface
	{
		_Utils::Observer* NewFrame;
		_Utils::Observer* PreRender;
		UpdateSequencer* UpdateSequencer;
		std::function<void(float)>* SandboxUpdateHook;

		GameLoop* GameLoop;
		_Log::MyLog* Log;
		Core_Clock* Clock;
		_Render::RenderInterface* RenderInterface;
		_Input::Input* Input;
		_ECS::ECS* ECS;
	};

	void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication);
}