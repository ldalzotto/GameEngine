#include "IntegrationTest.h"

#include "GameEngineApplication.h"

namespace _GameEngine::_Test
{
	_GameEngine::GameEngineApplication* IntegrationTest_begin()
	{
		return app_alloc();
	};
	
	void IntegrationTest_end(_GameEngine::GameEngineApplication* p_gameEngineApplication)
	{
		app_free(p_gameEngineApplication);
	};
}