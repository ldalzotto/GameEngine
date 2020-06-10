#include "SystemV2Factory.h"

#include "Utils/SortedSequencer/SortedSequencer.h"
#include "ECS/System.h"
#include "EngineSequencers/EngineSequencers.h"

namespace _GameEngine::_ECS_Impl
{
	void SystemV2Factory_allocSystemV2(_ECS::SystemV2AllocInfo* p_systemV2AllocInfo, UpdateSequencer* p_updateSequencer)
	{
		auto l_system = _ECS::SystemV2_alloc(p_systemV2AllocInfo);
		l_system->Update.Closure = l_system;
		_Utils::SortedSequencer_addOperation(&p_updateSequencer->UpdateSequencer, &l_system->Update);
	};
}