#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Entity.h"

#include "Functional/Sequencer/SortedSequencer.hpp"

namespace _Core
{
	struct SortedSequencer;
}

namespace _GameEngine::_ECS
{
	struct ECS;
}

namespace _GameEngine::_ECS
{
	typedef size_t SystemV2Key;

	struct SystemHeader
	{
		SystemV2Key SystemKey;
		ECS* ECS;
		::_Core::SortedSequencer* UpdateSequencer;
		::_Core::SortedSequencerOperation Update;
		_Core::CallbackT<void, void> OnSystemDestroyed;
	};

	void SystemHeader_init(SystemHeader* p_systemHeader, ECS* p_ecs, _Core::SortedSequencer* p_sequencer);
	void SystemHeader_free(SystemHeader* p_systemHeader);

	struct SystemContainerV2
	{
		_Core::VectorT<SystemHeader*> SystemV2;
	};

	void SystemContainerV2_alloc(SystemContainerV2* p_systemContainer);
	void SystemContainerV2_addSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2);
	void SystemContainerV2_removeSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2);
	SystemHeader* SystemContainerV2_getSystem(SystemContainerV2* p_systemContainer, SystemV2Key* p_key);
	void SystemContainerV2_free(SystemContainerV2* p_systemContainer);
}