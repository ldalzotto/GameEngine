#pragma once

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine
{
	struct GameEngineApplicationInterface;

	namespace _ECS { struct ECS; }
}

namespace _GameEngine
{
	extern _Utils::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY;	
	extern _Utils::SortedSequencerPriority EDITR_PRIORITY;


	struct UpdateSequencer
	{
		_ECS::ECS* ECS;
		_Utils::SortedSequencer UpdateSequencer;
	};

	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer);
	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
}