#include "IntegrationTest.h"

extern "C"
{
#include "GameEngineApplication.h"
}

namespace _GameEngine::_Test
{
	GameEngineApplication* IntegrationTest_begin()
	{
		return app_alloc();
	};
	
	void IntegrationTest_end(GameEngineApplication* p_gameEngineApplication)
	{
		app_free(p_gameEngineApplication);
	};
}