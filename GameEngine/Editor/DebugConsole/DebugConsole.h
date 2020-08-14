#pragma once

#if 0

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
}

namespace _GameEngineEditor
{
	struct DrawableWindows;
}

namespace _GameEngineEditor
{
	struct DebugConsole
	{
		_GameEngine::GameEngineApplicationInterface* GameEngineApplicationInterface;
		char CurrentWritingInput[256];
		DrawableWindows* DrawableWindows;
	};

	void DebugConsole_draw(DebugConsole* p_debugConsole);
}

#endif