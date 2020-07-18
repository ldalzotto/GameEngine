#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Entity.h"

#include "Functional/Sequencer/SortedSequencer.hpp"

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
		::_Core::SortedSequencerOperation Update;
		_Core::CallbackT<void, void> OnSystemDestroyed;
	};

	struct SystemV2
	{
		SystemV2Key SystemKey;
		::_Core::SortedSequencerOperation Update;
		void(*OnSystemDestroyed)(SystemV2* p_system);

		ECS* ECS;
		EntityConfigurableContainer EntityConfigurableContainer;
		void* Child;
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

	struct SystemContainerV2
	{
		_Core::VectorT<SystemHeader*> SystemV2;
	};

	void SystemContainerV2_alloc(SystemContainerV2* p_systemContainer);
	void SystemContainerV2_addSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2);
	void SystemContainerV2_removeSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2);
	void SystemContainerV2_free(SystemContainerV2* p_systemContainer);
}