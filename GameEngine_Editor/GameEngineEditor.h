#pragma once

#include "IMGuiRender/IMGuiRender.h"

extern "C"
{
#include "Functional/Callback/Callback.h"
}

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
		Callback OnPreRender;
		IMGuiRender IMGuiRender;
		DebugConsole DebugConsole;
		DrawableWindows DrawableWindows;
	};

	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
}