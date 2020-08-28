#include "GameEngineApplication.h"


#include "AppEvent/AppEvent.h"
#include "Log/Log.h"
#include "ECS.h"
#include "ECSEngine/Systems/CameraRenderSystem.h"
#include "ECSEngine/Systems/MeshDrawSystem.h"
#include "ECSEngine/GlobalEvents/ECSEngineGlobalEvents.h"
#include "Input/InputGlobalEvent.h"
#include "Memory/AllocationDebug.h"

ECSEngineGlobal_OnComponentDestroyed_Closure_TMP TMPClosure;

/// Game loop callback forward declaration
void GameEngineApplication_initializeSystems(GameEngineApplication* p_gameEngineApplication);
void app_newFrame(void* p_gameEngineApplication);
void app_update(void* p_closure, float p_delta);
void app_endOfUpdate(void* p_closure);
void app_render(void* p_closure);
void app_endOfFrame(void* p_closure);
///

GameEngineApplication* app_alloc()
{
	AllocationDebug_Alloc();
	
	GameEngineApplication* l_gameEngineApplication = (GameEngineApplication*)calloc(1, sizeof(GameEngineApplication));

	AllocationDebug_Write(l_gameEngineApplication, ALLOCATIONDEBUG_TYPE_ALLOC, __FILE__, __LINE__);

	AppEvent_initialize();
	InputGlobalEvent_Initialize();

	GameEngineApplicationInterface_build(&l_gameEngineApplication->GameEngineApplicationInterface, l_gameEngineApplication);

	MyLog_build(&l_gameEngineApplication->Log, &l_gameEngineApplication->Clock);
	Physics_alloc(&l_gameEngineApplication->Physics, &l_gameEngineApplication->Log);
	RenderV2_initialize(&l_gameEngineApplication->Render);
	Input_build(&l_gameEngineApplication->Input, &l_gameEngineApplication->Render.AppWindow, &l_gameEngineApplication->Log);
	GameLoop_build(&l_gameEngineApplication->GameLoop, 16000);

	ECS_Build(&l_gameEngineApplication->ECS, &l_gameEngineApplication->Log);
	TMPClosure.ECS = &l_gameEngineApplication->ECS;
	TMPClosure.RenderInterface = &l_gameEngineApplication->Render.RenderInterface;
	ECS_OnComponentDestroyedStaticCallback l_onComponentDestroyedCallback = { ECSEngineGlobal_OnComponentDestroyed, &TMPClosure };
	ECS_RegisterGlobalComponentDestroyedEvent(&l_gameEngineApplication->ECS, &l_onComponentDestroyedCallback);

#if GAMEENGINE_EDITOR
	GameEngineEditor_alloc(&l_gameEngineApplication->Editor, &l_gameEngineApplication->GameEngineApplicationInterface);
#endif

	GameLoop_SetnewFrameCallback(&l_gameEngineApplication->GameLoop, app_newFrame, l_gameEngineApplication);
	GameLoop_SetupdateCallback(&l_gameEngineApplication->GameLoop, app_update, l_gameEngineApplication);
	GameLoop_SetendOfUpdateCallback(&l_gameEngineApplication->GameLoop, app_endOfUpdate, l_gameEngineApplication);
	GameLoop_SetrenderCallback(&l_gameEngineApplication->GameLoop, app_render, l_gameEngineApplication);
	GameLoop_SetendOfFrameCallback(&l_gameEngineApplication->GameLoop, app_endOfFrame, l_gameEngineApplication);

	GameEngineApplication_initializeSystems(l_gameEngineApplication);
	return l_gameEngineApplication;
}

void GameEngineApplication_initializeSystems(GameEngineApplication* p_gameEngineApplication)
{
	CameraRenderSystem_Alloc(&p_gameEngineApplication->Systems.CameraRenderSystem, &p_gameEngineApplication->ECS);
	MeshDrawSystem_Alloc(&p_gameEngineApplication->Systems.MeshDrawSystem, &p_gameEngineApplication->ECS);
	PhysicsSystem_Alloc(&p_gameEngineApplication->Systems.PhysicsSystem, &p_gameEngineApplication->ECS);
};

void app_free(GameEngineApplication* p_app)
{

#if GAMEENGINE_EDITOR
	GameEngineEditor_free(&p_app->Editor, &p_app->GameEngineApplicationInterface);
#endif

	// We free the ecs before systems to push EntityFiltre events.
	ECS_Free(&p_app->ECS);

	CameraRenderSystem_Free(&p_app->Systems.CameraRenderSystem, &p_app->ECS);
	MeshDrawSystem_Free(&p_app->Systems.MeshDrawSystem, &p_app->ECS, &p_app->Render.RenderInterface);
	PhysicsSystem_Free(&p_app->Systems.PhysicsSystem, &p_app->ECS, &p_app->Physics.PhysicsInterface);

	GameLoop_free(&p_app->GameLoop);
	RenderV2_free(&p_app->Render);
	Physics_free(&p_app->Physics);
	Input_free(&p_app->Input);

	MyLog_free(&p_app->Log);

	InputGlobalEvent_Free();
	AppEvent_free();

	free(p_app);

	AllocationDebug_Free();
}

void app_mainLoop(GameEngineApplication* p_app)
{
	while (!Window_askedForClose(&p_app->Render.AppWindow))
	{
		AppEvent_pool();
		GameLoop_Update(&p_app->GameLoop);
	}
};

void app_newFrame(void* p_gameEngineApplication)
{
	GameEngineApplication* l_app = (GameEngineApplication*)p_gameEngineApplication;
	Clock_NewFrame(&l_app->Clock);
	Input_newFrame(&l_app->Input);
};

void app_update(void* p_closure, float p_delta)
{
	GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
	Clock_NewUpdate(&l_app->Clock, p_delta);

	ECS_GlobalEvents_ProcessMessages(&l_app->ECS);

	CameraRenderSystem_Update(&l_app->Systems.CameraRenderSystem);
	PhysicsSystem_Update(&l_app->Systems.PhysicsSystem, &l_app->Physics.PhysicsInterface);
	ECS_GlobalEvents_ProcessMessages(&l_app->ECS);

	GameEngineEditor_update(&l_app->Editor);
	ECS_GlobalEvents_ProcessMessages(&l_app->ECS);

	if (l_app->Hooks.UpdateAfter)
	{
		l_app->Hooks.UpdateAfter(&l_app->GameEngineApplicationInterface, l_app->Hooks.UpdateAfterClosure);
	}

	MeshDrawSystem_Update(&l_app->Systems.MeshDrawSystem, &l_app->Render.RenderInterface);

	ECS_GlobalEvents_ProcessMessages(&l_app->ECS);

	// _ECS::ECSEventQueue_processMessages(&l_app->ECS.EventQueue);
};

void app_endOfUpdate(void* p_closure)
{
	GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
};

void app_render(void* p_closure)
{
	GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
	RenderV2_render(&l_app->Render);
};

void app_endOfFrame(void* p_closure)
{
	GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
	MyLog_processLogs(&l_app->Log);
};

int app_run(GameEngineApplication* p_app)
{
	app_mainLoop(p_app);
	app_free(p_app);
	return EXIT_SUCCESS;

};