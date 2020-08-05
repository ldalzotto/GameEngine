#pragma once

namespace _Core
{
	struct AppEventParams;
}

namespace _GameEngine
{
	struct GameEngineApplication;
}

namespace _GameEngine::_Test
{
	_GameEngine::GameEngineApplication* IntegrationTest_begin(_Core::AppEventParams* p_params);
	void IntegrationTest_end(_GameEngine::GameEngineApplication* p_gameEngineApplication);
}