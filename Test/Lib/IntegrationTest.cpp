#include "IntegrationTest.h"

#include "GameEngineApplication.h"

namespace _GameEngine::_Test
{
	_GameEngine::GameEngineApplication* IntegrationTest_begin(_Core::AppEventParams* p_params)
	{
		return app_alloc(p_params);
	};
	
	void IntegrationTest_end(_GameEngine::GameEngineApplication* p_gameEngineApplication)
	{
		app_free(p_gameEngineApplication);
	};
}