#pragma once

namespace _GameEngine {
	struct  GameEngineApplication;
}

struct SandboxApplication
{
	_GameEngine::GameEngineApplication* App;
};

SandboxApplication SanboxApplication_alloc();
int SandboxApplication_run(SandboxApplication* p_sandboxApplication);