#pragma once

#include "IMGuiRender/IMGuiRender.h"

#include "Functional/Callback/CallbackT.hpp"

#include "DebugConsole/DebugConsole.h"
#include "IMGuiRender/DrawableWindow.h"
#include "ObjectSelection/EntitySelection.h"

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
		DrawableWindows DrawableWindows;
		EntitySelection EntitySelection;
	};

	void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_update(GameEngineEditor* p_gameEngineEditor);
}