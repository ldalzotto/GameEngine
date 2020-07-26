#include "System.h"

#include <cstdlib>

#include "ECS.h"
#include "Algorithm/Compare/CompareAlgorithmT.hpp"
#include "Functional/Sequencer/SortedSequencer.hpp"

namespace _GameEngine::_ECS
{
	bool SystemV2Key_comparator(SystemHeader** p_left, SystemV2Key* p_right, void*)
	{
		return (*p_left)->SystemKey == *p_right;
	};

	bool SystemHeader_comparator(SystemHeader** left, SystemHeader** right, void*)
	{
		return *left == *right;
	};
}

namespace _GameEngine::_ECS
{
	void SystemEvent_triggerOnSystemAdded(SystemEvents* p_systemEvents, SystemHeader* p_systemHeader);

	void SystemHeader_init(SystemHeader* p_systemHeader, ECS* p_ecs, _Core::SortedSequencer* p_sequencer)
	{
		p_systemHeader->ECS = p_ecs;
		p_systemHeader->UpdateSequencer = p_sequencer;
		_Core::SortedSequencer_addOperation(p_sequencer, &p_systemHeader->Update);
		SystemContainerV2_addSystemV2(p_ecs, p_systemHeader);
	};

	void SystemHeader_free(SystemHeader* p_systemHeader)
	{
		_Core::SortedSequencer_removeOperation(p_systemHeader->UpdateSequencer, &p_systemHeader->Update);
		SystemContainerV2_removeSystemV2(&p_systemHeader->ECS->SystemContainerV2, p_systemHeader);
	};

	void SystemContainerV2_alloc(SystemContainerV2* p_systemContainer)
	{
		_Core::VectorT_alloc(&p_systemContainer->SystemV2, 0);
	};

	void SystemContainerV2_addSystemV2(ECS* p_ecs, SystemHeader* p_systemV2)
	{
		_Core::VectorT_pushBack(&p_ecs->SystemContainerV2.SystemV2, &p_systemV2);
		SystemEvent_triggerOnSystemAdded(&p_ecs->SystemEvents, p_systemV2);
	};

	void SystemContainerV2_removeSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2)
	{
		_Core::VectorT_eraseCompare(&p_systemContainer->SystemV2, _Core::ComparatorT<SystemHeader*, SystemHeader*, void>{ SystemHeader_comparator, &p_systemV2 });
	};

	SystemHeader* SystemContainerV2_getSystem(SystemContainerV2* p_systemContainer, SystemV2Key* p_key)
	{
		return *_Core::CompareT_find(_Core::VectorT_buildIterator(&p_systemContainer->SystemV2), _Core::ComparatorT<SystemHeader*, SystemV2Key, void>{SystemV2Key_comparator, p_key}).Current;
	};

	void SystemContainerV2_free(SystemContainerV2* p_systemContainer)
	{
		_Core::VectorReverseIteratorT<SystemHeader*> l_it = _Core::VectorT_buildReverseIterator(&p_systemContainer->SystemV2);
		while (_Core::VectorReverseIteratorT_moveNext(&l_it))
		{
			_Core::CallbackT_call(&(*l_it.Current)->OnSystemDestroyed, (void*)nullptr);
		}
		_Core::VectorT_free(&p_systemContainer->SystemV2);
	};

	void SystemEvents_free(SystemEvents* p_systemEvents)
	{
		for (auto&& l_onSystemAddedEventEntry : p_systemEvents->OnSystemAddedEvents)
		{
			_Core::ObserverT_free(&l_onSystemAddedEventEntry.second);
		}
	};

	void SystemEvents_registerOnSystemAdded(SystemEvents* p_systemEvents, SystemV2Key l_key, _Core::CallbackT<void, SystemHeader*>* p_callback)
	{
		if (!p_systemEvents->OnSystemAddedEvents.contains(l_key))
		{
			_Core::ObserverT<SystemHeader*> l_allocatedObserver;
			_Core::ObserverT_alloc(&l_allocatedObserver);
			p_systemEvents->OnSystemAddedEvents[l_key] = l_allocatedObserver;
		}

		_Core::ObserverT_register(&p_systemEvents->OnSystemAddedEvents[l_key], p_callback);
	};

	void SystemEvent_triggerOnSystemAdded(SystemEvents* p_systemEvents, SystemHeader* p_systemHeader)
	{
		if (p_systemEvents->OnSystemAddedEvents.contains(p_systemHeader->SystemKey))
		{
			_Core::ObserverT_broadcast(&p_systemEvents->OnSystemAddedEvents[p_systemHeader->SystemKey], &p_systemHeader);
		}
	};
}