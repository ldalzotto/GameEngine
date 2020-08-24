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
	namespace _Input { struct Input; }
}

typedef struct RenderV2Interface_TYP RenderV2Interface;
typedef struct ECS_TYP ECS;
typedef struct Log_TYP Log;
typedef struct PhysicsInterface_TYP PhysicsInterface;

typedef struct CameraRenderSystem_TYP CameraRenderSystem;
typedef struct MeshDrawSystem_TYP MeshDrawSystem;
typedef struct PhysicsSystem_TYP PhysicsSystem;

namespace _GameEngine
{
	

	struct GameEngineApplicationSystemsInterface
	{
		CameraRenderSystem* CameraRenderSystem;
		MeshDrawSystem* MeshDrawSystem;
		PhysicsSystem* PhysicsSystem;
	};

	struct GameEngineApplicationInterface
	{
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface>* NewFrame;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface>* PreRender;
		std::function<void(float)>* SandboxUpdateHook;

		GameLoop* GameLoop;
		Log* Log;
		Clock* Clock;
		PhysicsInterface* PhysicsInterface;
		RenderV2Interface* RenderInterface;
		_Input::Input* Input;
		ECS* ECS;

		GameEngineApplicationSystemsInterface GameEngineApplicationSystems;
	};

	void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication);
}