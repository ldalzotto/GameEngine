#pragma once

typedef struct GameEngineApplication_TYP GameEngineApplication;

namespace _GameEngine::_Test
{
	GameEngineApplication* IntegrationTest_begin();
	void IntegrationTest_end(GameEngineApplication* p_gameEngineApplication);
}