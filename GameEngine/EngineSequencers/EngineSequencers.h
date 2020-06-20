#pragma once

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
}

namespace _GameEngine
{
	extern _Utils::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY;	
	extern _Utils::SortedSequencerPriority EDITR_PRIORITY;


	struct UpdateSequencer
	{
		_Utils::SortedSequencer UpdateSequencer;
	};

	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
}