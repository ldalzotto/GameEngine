#pragma once

#include "Functional/Sequencer/SortedSequencerT.hpp"

namespace _GameEngine
{
	struct GameEngineApplicationInterface;

	namespace _ECS { struct ECS; }
}

namespace _GameEngine
{
	/* When all data needed for graphics are pushed to the graphics engine */
	extern ::_Core::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY;
	extern ::_Core::SortedSequencerPriority EDITOR_PRIORITY;
	extern ::_Core::SortedSequencerPriority END_OF_UPDATE;

	struct UpdateSequencer
	{
		_ECS::ECS* ECS;
		::_Core::SortedSequencerT<GameEngineApplicationInterface> UpdateSequencer;
	};

	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer);
	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface);
}