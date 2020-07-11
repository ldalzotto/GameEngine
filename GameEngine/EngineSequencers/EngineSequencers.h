#pragma once

extern "C"
{
#include "Functional/Sequencer/SortedSequencer.h"
}

namespace _GameEngine
{
	struct GameEngineApplicationInterface;

	namespace _ECS { struct ECS; }
}

namespace _GameEngine
{
	/* When all data needed for graphics are pushed to the graphics engine */
	extern SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY;
	extern SortedSequencerPriority EDITOR_PRIORITY;

	struct UpdateSequencer
	{
		_ECS::ECS* ECS;
		Core_SortedSequencer UpdateSequencer;
	};

	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer);
	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
}