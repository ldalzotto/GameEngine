#include "GameEngineApplication.h"

namespace _GameEngine
{
	/// Game loop callback forward declaration
	void app_newFrame(void* p_gameEngineApplication);
	void app_update(void* p_closure, float p_delta);
	void app_render(void* p_closure);
	void app_endOfFrame(void* p_closure);
	///

	GameEngineApplication* app_alloc(const std::function<void(float)>& p_sandboxUpdateHook)
	{
		GameEngineApplication* l_gameEngineApplication = new GameEngineApplication();
		l_gameEngineApplication->SandboxUpdateHook = p_sandboxUpdateHook;

		GameEngineApplicationInterface_build(&l_gameEngineApplication->GameEngineApplicationInterface, l_gameEngineApplication);

		_Log::Log_alloc();
		_Log::MyLog_build(&l_gameEngineApplication->Log, &l_gameEngineApplication->Clock);
		_Render::Render_build(&l_gameEngineApplication->Render);
		_Input::Input_build(&l_gameEngineApplication->Input, &l_gameEngineApplication->Render.Window);
		_GameLoop::GameLoop_build(&l_gameEngineApplication->GameLoop, 16000);
		_ECS::EntityComponent_build(&l_gameEngineApplication->ECS, &l_gameEngineApplication->UpdateSequencer);

		_GameLoop::set_newFrameCallback(&l_gameEngineApplication->GameLoop, app_newFrame, l_gameEngineApplication);
		_GameLoop::set_updateCallback(&l_gameEngineApplication->GameLoop, app_update, l_gameEngineApplication);
		_GameLoop::set_renderCallback(&l_gameEngineApplication->GameLoop, app_render, l_gameEngineApplication);
		_GameLoop::set_endOfFrameCallback(&l_gameEngineApplication->GameLoop, app_endOfFrame, l_gameEngineApplication);
		return l_gameEngineApplication;
	}

	void app_free(GameEngineApplication* p_app)
	{
		_ECS::EntityComponent_free(&p_app->ECS);
		_GameLoop::GameLoop_free(&p_app->GameLoop);
		_Render::Render_free(&p_app->Render);
		_Input::Input_free(&p_app->Input);

		_Log::Log_free(&_Log::LogInstance);
		_Log::MyLog_free(&p_app->Log);

		delete p_app;
	}

	void app_mainLoop(GameEngineApplication* p_app)
	{
		while (!Window_askedForClose(&p_app->Render.Window))
		{
			glfwPollEvents();
			_GameLoop::update(&p_app->GameLoop);
		}
	};

	void app_newFrame(void* p_gameEngineApplication)
	{
		GameEngineApplication* l_app = (GameEngineApplication*)p_gameEngineApplication;
		_Clock::Clock_newFrame(&l_app->Clock);
		_Input::Input_update(&l_app->Input);
		_Utils::Observer_broadcast(&l_app->NewFrame, &l_app->GameEngineApplicationInterface);
		//_Log::MyLog_pushLog(&l_app->Log, _Log::LogLevel::WARN, "Hello");
	};

	void app_update(void* p_closure, float p_delta)
	{
		GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
		_Clock::Clock_newUpdate(&l_app->Clock, p_delta);
		l_app->SandboxUpdateHook(p_delta);
		UpdateSequencer_execute(&l_app->UpdateSequencer, &l_app->GameEngineApplicationInterface);
	};

	void app_render(void* p_closure)
	{
		GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
		_Utils::Observer_broadcast(&l_app->PreRender, l_app);
		Render_render(&l_app->Render);
	};

	void app_endOfFrame(void* p_closure)
	{
		GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
		_Log::MyLog_processLogs(&l_app->Log);
	};

	void app_cleanup(GameEngineApplication* p_app)
	{
		Window_closeWindow(&p_app->Render.Window);
	};

	void app_run(GameEngineApplication* p_app)
	{
		app_mainLoop(p_app);
		app_cleanup(p_app);
	};
} // namespace _GameEngine