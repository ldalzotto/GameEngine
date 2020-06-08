#pragma once


namespace _GameEngineEditor
{
	struct DrawableWindows;
}

namespace _GameEngineEditor
{
	struct DebugConsole
	{
		char CurrentWritingInput[256];
		DrawableWindows* DrawableWindows;
	};

	void DebugConsole_draw(DebugConsole* p_debugConsole);
}