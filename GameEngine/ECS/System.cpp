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
	void SystemHeader_init(SystemHeader* p_systemHeader, ECS* p_ecs, _Core::SortedSequencer* p_sequencer)
	{
		_Core::SortedSequencer_addOperation(p_sequencer, &p_systemHeader->Update);
		SystemContainerV2_addSystemV2(&p_ecs->SystemContainerV2, p_systemHeader);
	};

	void SystemHeader_free(SystemHeader* p_systemHeader)
	{
		// TODO, remove sorted sequencer
		SystemContainerV2_removeSystemV2(&p_systemHeader->ECS->SystemContainerV2, p_systemHeader);
	};

	void SystemContainerV2_alloc(SystemContainerV2* p_systemContainer)
	{
		_Core::VectorT_alloc(&p_systemContainer->SystemV2, 0);
	};
	void SystemContainerV2_addSystemV2(SystemContainerV2* p_systemContainer, SystemHeader* p_systemV2)
	{
		_Core::VectorT_pushBack(&p_systemContainer->SystemV2, &p_systemV2);
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
}