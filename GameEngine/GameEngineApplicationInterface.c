#include "GameEngineApplicationInterface.h"
#include "GameEngineApplication.h"


void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication)
{

	p_interface->GameLoop = &p_gameEngineApplication->GameLoop;
	p_interface->Log = &p_gameEngineApplication->Log;
	p_interface->Clock = &p_gameEngineApplication->Clock;
	p_interface->PhysicsInterface = &p_gameEngineApplication->Physics.PhysicsInterface;
	p_interface->RenderInterface = &p_gameEngineApplication->Render.RenderInterface;
	p_interface->Input = &p_gameEngineApplication->Input;
	p_interface->ECS = &p_gameEngineApplication->ECS;

	p_interface->GameEngineApplicationSystems.CameraRenderSystem = &p_gameEngineApplication->Systems.CameraRenderSystem;
	p_interface->GameEngineApplicationSystems.MeshDrawSystem = &p_gameEngineApplication->Systems.MeshDrawSystem;
	p_interface->GameEngineApplicationSystems.PhysicsSystem = &p_gameEngineApplication->Systems.PhysicsSystem;
};