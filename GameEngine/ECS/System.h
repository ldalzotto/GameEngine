#pragma once

#include "DataStructures/VectorT.h"
#include "Entity.h"
#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_ECS
{
	struct ECS;
}

namespace _GameEngine::_ECS
{
	struct SystemV2
	{
		ECS* ECS;
		EntityConfigurableContainer EntityConfigurableContainer;
		_Utils::SortedSequencerOperation Update;
	};

	struct SystemV2AllocInfo
	{
		ECS* ECS;
		EntityConfigurableContainerInitInfo EntityConfigurableContainerInitInfo;
		_Utils::SortedSequencerOperation Update;
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
	void SystemContainer_free(SystemContainer* p_systemContainer);
}