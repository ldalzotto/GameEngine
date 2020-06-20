#pragma once

#include "IMGuiRender/IMGuiRender.h"
#include "Utils/Observer/Observer.h"
#include "DebugConsole/DebugConsole.h"
#include "IMGuiRender/DrawableWindow.h"

using namespace _GameEngine;

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
}

namespace _GameEngineEditor
{
	struct GameEngineEditor
	{
		GameEngineApplicationInterface* GameEngineApplicationInterface;
		_Utils::Subject OnPreRender;
		IMGuiRender IMGuiRender;
		DebugConsole DebugConsole;
		DrawableWindows DrawableWindows;
	};

	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
}