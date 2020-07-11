#include "EngineSequencers.h"

#include "GameEngineApplicationInterface.h"
#include "ECS/ECS.h"

extern "C"
{
#include "Functional/Sequencer/SortedSequencer.h"
}

namespace _GameEngine
{
	SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY = 600;
	SortedSequencerPriority EDITOR_PRIORITY = 500;

	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		Core_SortedSequencer_alloc(&p_udpateSequencer->UpdateSequencer);
		p_udpateSequencer->ECS = p_gameEngineInterface->ECS;
	};
	
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer)
	{
		Core_SortedSequencer_free(&p_udpateSequencer->UpdateSequencer);
	};

	void updateSequencer_betweenEveryUpdates(void* p_ecs, void* p_null)
	{
		_ECS::ECS* l_ecs = (_ECS::ECS*)p_ecs;
		_ECS::ECSEventQueue_processMessages(&l_ecs->EventQueue);
	};

	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		Core_SortedSequencer_execute_afterCallback(&p_updateSequencer->UpdateSequencer, p_gameEngineInterface, { updateSequencer_betweenEveryUpdates, p_gameEngineInterface->ECS });
	};
}