#include "EngineSequencers.h"

#include "GameEngineApplicationInterface.h"
#include "ECS/ECS.h"

namespace _GameEngine
{
	::_Core::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY = 600;
	::_Core::SortedSequencerPriority EDITOR_PRIORITY = 500;

	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		::_Core::SortedSequencerT_alloc(&p_udpateSequencer->UpdateSequencer);
		p_udpateSequencer->ECS = p_gameEngineInterface->ECS;
	};
	
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer)
	{
		::_Core::SortedSequencerT_free(&p_udpateSequencer->UpdateSequencer);
	};

	void updateSequencer_betweenEveryUpdates(_ECS::ECS* p_ecs, void* p_null)
	{
		_ECS::ECSEventQueue_processMessages(&p_ecs->EventQueue);
	};

	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		::_Core::CallbackT<_ECS::ECS, void> l_afterExecutioncalback = { updateSequencer_betweenEveryUpdates, p_gameEngineInterface->ECS };
		::_Core::SortedSequencerT_execute_afterCallback(&p_updateSequencer->UpdateSequencer, p_gameEngineInterface, &l_afterExecutioncalback);
	};
}