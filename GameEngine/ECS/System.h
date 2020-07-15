#pragma once

#include "DataStructures/VectorT.h"
#include "Entity.h"

#include "Functional/Sequencer/SortedSequencer.hpp"

namespace _GameEngine::_ECS
{
	struct ECS;
}

namespace _GameEngine::_ECS
{
	typedef size_t SystemV2Key;

	struct SystemV2
	{
		ECS* ECS;
		SystemV2Key SystemKey;
		EntityConfigurableContainer EntityConfigurableContainer;
		::_Core::SortedSequencerOperation Update;

		void* Child;
		void(*OnSystemDestroyed)(SystemV2* p_system);
	};

	struct SystemV2AllocInfo
	{
		ECS* ECS;
		SystemV2Key SystemKey;
		EntityConfigurableContainerInitInfo EntityConfigurableContainerInitInfo;
		::_Core::SortedSequencerOperation Update;

		void* Child;
		void(*OnSystemDestroyed)(SystemV2* p_system);
	};

	SystemV2* SystemV2_alloc(SystemV2AllocInfo* p_systemV2AllocInfo);
	void SystemV2_free(SystemV2** p_systemV2);

	struct SystemContainer
	{
		_Core::VectorT<SystemV2*> SystemsV2;
	};

	void SystemContainer_alloc(SystemContainer* p_systemContainer);
	void SystemContainer_addSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2);
	void SystemContainer_removeSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2);
	SystemV2* SystemContainer_getSystem(SystemContainer* p_systemContainer, SystemV2Key* p_key);
	void SystemContainer_free(SystemContainer* p_systemContainer);
}