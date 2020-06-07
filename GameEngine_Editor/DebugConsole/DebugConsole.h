#pragma once

namespace _GameEngineEditor
{
	struct DebugConsole
	{
		char CurrentWritingInput[256];
	};

	void DebugConsole_draw(DebugConsole* p_debugConsole);
}