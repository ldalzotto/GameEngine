#pragma once

namespace _GameEngine {
	struct  GameEngineApplication;
}
namespace _GameEngineEditor
{
	struct GameEngineEditor;
}

struct SandboxApplication
{
	_GameEngine::GameEngineApplication* App;
	_GameEngineEditor::GameEngineEditor* App_Editor;
};

SandboxApplication SanboxApplication_alloc();
int SandboxApplication_run(SandboxApplication* p_sandboxApplication);