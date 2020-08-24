#pragma once

typedef struct Clock_TYP Clock;

typedef struct GameEngineApplicationInterface_TYP GameEngineApplicationInterface;
typedef struct GameEngineApplication_TYP GameEngineApplication;

typedef struct RenderV2Interface_TYP RenderV2Interface;
typedef struct ECS_TYP ECS;
typedef struct Log_TYP Log;
typedef struct PhysicsInterface_TYP PhysicsInterface;

typedef struct GameLoop_TYP GameLoop;
typedef struct CameraRenderSystem_TYP CameraRenderSystem;
typedef struct MeshDrawSystem_TYP MeshDrawSystem;
typedef struct PhysicsSystem_TYP PhysicsSystem;
typedef struct Input_TYP Input;

typedef struct GameEngineApplicationSystemsInterface_TYP
{
	CameraRenderSystem* CameraRenderSystem;
	MeshDrawSystem* MeshDrawSystem;
	PhysicsSystem* PhysicsSystem;
}GameEngineApplicationSystemsInterface, * GameEngineApplicationSystemsInterface_PTR;

typedef struct GameEngineApplicationInterface_TYP
{
	GameLoop* GameLoop;
	Log* Log;
	Clock* Clock;
	PhysicsInterface* PhysicsInterface;
	RenderV2Interface* RenderInterface;
	Input* Input;
	ECS* ECS;

	GameEngineApplicationSystemsInterface GameEngineApplicationSystems;
}GameEngineApplicationInterface, * GameEngineApplicationInterface_PTR;

void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication);