#include "EngineSequencers.h"

namespace _GameEngine
{
	_Utils::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY = 600;
	_Utils::SortedSequencerPriority EDITR_PRIORITY = 500;

	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, float p_float)
	{
		_Utils::SortedSequencer_execute(&p_updateSequencer->UpdateSequencer, &p_float);
	};
}