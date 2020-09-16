#pragma once

typedef struct GameEngineApplication_TYP GameEngineApplication;

GameEngineApplication* IntegrationTest_begin(const char* p_executablePath);
void IntegrationTest_end(GameEngineApplication* p_gameEngineApplication);
