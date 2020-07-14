#pragma once

#include <functional>

struct Core_Observer;

namespace _Core
{
	struct Log;
	struct Clock;
}

namespace _GameEngine
{
	struct GameEngineApplication;
	struct GameLoop;
	struct UpdateSequencer;
	namespace _Render { struct RenderInterface; }
	namespace _Input { struct Input; }
	namespace _ECS { struct ECS; }
}

namespace _GameEngine
{
	struct GameEngineApplicationInterface
	{
		Core_Observer* NewFrame;
		Core_Observer* PreRender;
		UpdateSequencer* UpdateSequencer;
		std::function<void(float)>* SandboxUpdateHook;

		GameLoop* GameLoop;
		::_Core::Log* Log;
		::_Core::Clock* Clock;
		_Render::RenderInterface* RenderInterface;
		_Input::Input* Input;
		_ECS::ECS* ECS;
	};

	void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication);
}