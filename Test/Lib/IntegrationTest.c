#include "IntegrationTest.h"

#include "GameEngineApplication.h"


GameEngineApplication* IntegrationTest_begin(const char* p_executablePath)
{
	return app_alloc(p_executablePath);
};

void IntegrationTest_end(GameEngineApplication* p_gameEngineApplication)
{
	app_free(p_gameEngineApplication);
};