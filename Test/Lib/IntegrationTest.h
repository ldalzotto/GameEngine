#pragma once

namespace _GameEngine
{
	struct GameEngineApplication;
}

namespace _GameEngine::_Test
{
	_GameEngine::GameEngineApplication* IntegrationTest_begin();
	void IntegrationTest_end(_GameEngine::GameEngineApplication* p_gameEngineApplication);
}