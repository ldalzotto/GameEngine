#pragma once


#include "Functional/Callback/CallbackT.hpp"

#include "ObjectSelection/EntitySelection.h"

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
}

using namespace _GameEngine;

namespace _GameEngineEditor
{
	struct GameEngineEditor
	{
		GameEngineApplicationInterface* GameEngineApplicationInterface;
		EntitySelection EntitySelection;
	};

	void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_update(GameEngineEditor* p_gameEngineEditor);
}