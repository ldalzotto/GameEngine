#include "GameEngineApplicationInterface.h"
#include "GameEngineApplication.h"

#include "Render/Render.h"

/*
	{
		_Utils::Observer* NewFrame;
		_Utils::Observer* PreRender;
		UpdateSequencer* UpdateSequencer;
		std::function<void(float)>* SandboxUpdateHook;
*/

namespace _GameEngine
{
	void GameEngineApplicationInterface_build(GameEngineApplicationInterface* p_interface, GameEngineApplication* p_gameEngineApplication)
	{
		p_interface->NewFrame = &p_gameEngineApplication->NewFrame;
		p_interface->PreRender = &p_gameEngineApplication->PreRender;
		p_interface->UpdateSequencer = &p_gameEngineApplication->UpdateSequencer;
		p_interface->SandboxUpdateHook = &p_gameEngineApplication->SandboxUpdateHook;

		p_interface->GameLoop = &p_gameEngineApplication->GameLoop;
		p_interface->Log = &p_gameEngineApplication->Log;
		p_interface->Clock = &p_gameEngineApplication->Clock;
		p_interface->RenderInterface = &p_gameEngineApplication->Render.RenderInterface;
		p_interface->Input = &p_gameEngineApplication->Input;
		p_interface->ECS = &p_gameEngineApplication->ECS;
	};
}