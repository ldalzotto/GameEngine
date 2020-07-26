#pragma once

#include <unordered_map>

#include "Entity.h"

#include "DataStructures/Specifications/VectorT.hpp"
#include "Functional/Sequencer/SortedSequencer.hpp"
#include "Functional/Callback/ObserverT.hpp"

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
	void SystemContainerV2_addSystemV2(ECS* p_ecs, SystemHeader* p_systemV2);
	void SystemContainerV2_removeSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2);
	SystemHeader* SystemContainerV2_getSystem(SystemContainerV2* p_systemContainer, SystemV2Key* p_key);
	void SystemContainerV2_free(SystemContainerV2* p_systemContainer);

	struct SystemEvents
	{
		std::unordered_map<SystemV2Key, _Core::ObserverT<SystemHeader*>> OnSystemAddedEvents;
	};

	void SystemEvents_free(SystemEvents* p_systemEvents);

	void SystemEvents_registerOnSystemAdded(SystemEvents* p_systemEvents, SystemV2Key l_key, _Core::CallbackT<void, SystemHeader*>* p_callback);
}