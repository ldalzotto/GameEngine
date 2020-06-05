#pragma once

#include "Utils/SortedSequencer/SortedSequencer.h"

using namespace _GameEngine;

namespace _GameEngine
{
	struct GameEngineApplication;
}

namespace _GameEngineEditor
{
	struct GameEngineEditor
	{
		_Utils::SortedSequencerOperation EngineEditorOperation;
	};

	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplication* p_gameEngineApplication);
	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor);
}