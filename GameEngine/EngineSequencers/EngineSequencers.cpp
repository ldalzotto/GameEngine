#include "EngineSequencers.h"

#include "GameEngineApplicationInterface.h"
#include "ECS/ECS.h"

namespace _GameEngine
{
	_Utils::SortedSequencerPriority UPDATE_PUSH_TO_RENDER_PRIORITY = 600;
	_Utils::SortedSequencerPriority EDITR_PRIORITY = 500;


	void UpdateSequencer_alloc(UpdateSequencer* p_udpateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		_Utils::SortedSequencer_alloc(&p_udpateSequencer->UpdateSequencer);
		p_udpateSequencer->ECS = p_gameEngineInterface->ECS;
	};
	
	void UpdateSequencer_free(UpdateSequencer* p_udpateSequencer)
	{
		_Utils::SortedSequender_free(&p_udpateSequencer->UpdateSequencer);
	};

	void updateSequencer_betweenEveryUpdates(void* p_ecs)
	{
		_ECS::ECS* l_ecs = (_ECS::ECS*)p_ecs;
		_ECS::ECSEventQueue_processMessages(&l_ecs->EventQueue);
	};

	void UpdateSequencer_execute(UpdateSequencer* p_updateSequencer, GameEngineApplicationInterface* p_gameEngineInterface)
	{
		_Utils::SortedSequencer_execute(&p_updateSequencer->UpdateSequencer, p_gameEngineInterface, updateSequencer_betweenEveryUpdates, p_gameEngineInterface->ECS);
	};
}