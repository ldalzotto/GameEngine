#pragma once

#include "GameEngineApplicationConfiguration.h"

#include "Clock/Clock.h"
#include "RenderV2.h"
#include "Log/Log_def.h"
#include "ECS_def.h"
#include "ECSEngine/Systems/CameraRenderSystem_def.h"
#include "ECSEngine/Systems/MeshDrawSystem_def.h"
#include "Physics/Physics.h"
#include "ECSEngine/Systems/PhysicsSystem.h"
#include "Input/InputV2.h"
#include "GameLoop/GameLoop.h"


#if GAMEENGINE_EDITOR
#include "Editor/GameEngineEditor.h"
#endif

#include "GameEngineApplicationInterface.h"

typedef struct GameEngineApplicationSystems_TYP
{
	CameraRenderSystem CameraRenderSystem;
	MeshDrawSystem MeshDrawSystem;
	PhysicsSystem PhysicsSystem;
}GameEngineApplicationSystems, * GameEngineApplicationSystems_PTR;

typedef struct GameEngineApplicationHooks_TYP
{
	void(*UpdateAfter)(GameEngineApplicationInterface* p_gameEngineApplication, void*);
	void* UpdateAfterClosure;
}GameEngineApplicationHooks, * GameEngineApplicationHooks_PTR;

typedef struct GameEngineApplication_TYP
{
	GameEngineApplicationInterface GameEngineApplicationInterface;

	GameLoop GameLoop;
	Log Log;
	Clock Clock;
	Physics Physics;
	RenderV2 Render;
	Input Input;
	ECS ECS;

	GameEngineApplicationSystems Systems;
	GameEngineApplicationHooks Hooks;

#if GAMEENGINE_EDITOR
	GameEngineEditor Editor;
#endif
}GameEngineApplication, * GameEngineApplication_PTR;

GameEngineApplication* app_alloc();
void app_free(GameEngineApplication* p_app);
void app_mainLoop(GameEngineApplication* p_app);
int app_run(GameEngineApplication* p_app);
