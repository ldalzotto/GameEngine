#include "EngineSequencers.h"

namespace _GameEngine
{
	_Utils::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY = 600;
	_Utils::SortedSequencerPriority EDITR_PRIORITY = 500;


	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer)
	{
		_Utils::SortedSequencer_alloc(&p_udpateSequencer->UpdateSequencer);
	};
	
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer)
	{
		_Utils::SortedSequender_free(&p_udpateSequencer->UpdateSequencer);
	};

	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		_Utils::SortedSequencer_execute(&p_updateSequencer->UpdateSequencer, p_gameEngineInterface);
	};
}