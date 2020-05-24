#include "GameEngineApplication.h"

namespace _GameEngine
{

	/// Game loop callback forward declaration
	void app_update(void* p_closure, float p_delta);
	void app_render(void* p_closure);
	///

	GameEngineApplication* app_alloc()
	{
		return new GameEngineApplication();
	}

	void app_init(GameEngineApplication* p_app)
	{
		_Log::Log_alloc();
		p_app->EntityComponent = _ECS::EntityComponent_alloc();
		p_app->Render = _Render::Render_alloc();
		p_app->GameLoop = _GameLoop::alloc(16000);
		_GameLoop::set_updateCallback(p_app->GameLoop, app_update, p_app);
		_GameLoop::set_renderCallback(p_app->GameLoop, app_render, p_app);
	};

	void app_free(GameEngineApplication* p_app)
	{
		_GameLoop::free(&p_app->GameLoop);
		_Render::Render_free(&p_app->Render);
		_ECS::EntityComponent_free(&p_app->EntityComponent);
		_Log::Log_free(&_Log::LogInstance);
		delete p_app;
	}

	void app_mainLoop(GameEngineApplication* p_app)
	{
		while (!Window_askedForClose(&p_app->Render->Window))
		{
			glfwPollEvents();
			_GameLoop::update(p_app->GameLoop);
		}
	};

	void app_update(void* p_closure, float p_delta)
	{
		GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
		_Log::LogInstance->CoreLogger->info("Hello from update : " + std::to_string(p_delta));
	};

	void app_render(void* p_closure)
	{
		GameEngineApplication* l_app = (GameEngineApplication*)p_closure;
		_Log::LogInstance->CoreLogger->info("Hello from render.");
		Render_render(l_app->Render);
	};

	void app_cleanup(GameEngineApplication* p_app)
	{
		Window_closeWindow(&p_app->Render->Window);
	};

	void app_run(GameEngineApplication* p_app)
	{
		app_init(p_app);
		app_mainLoop(p_app);
		app_cleanup(p_app);
	};
} // namespace _GameEngine