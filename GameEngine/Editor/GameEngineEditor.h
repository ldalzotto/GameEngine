#pragma once

#include "IMGuiRender/IMGuiRender.h"

#include "Functional/Callback/CallbackT.hpp"

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
		_Core::CallbackT<GameEngineEditor, void> OnPreRender;
		IMGuiRender IMGuiRender;
		DebugConsole DebugConsole;
		DrawableWindows DrawableWindows;
	};

	void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
}