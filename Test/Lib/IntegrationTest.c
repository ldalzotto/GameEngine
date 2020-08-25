#include "IntegrationTest.h"

#include "GameEngineApplication.h"


GameEngineApplication* IntegrationTest_begin()
{
	return app_alloc();
};

void IntegrationTest_end(GameEngineApplication* p_gameEngineApplication)
{
	app_free(p_gameEngineApplication);
};