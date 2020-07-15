#include "System.h"

#include <cstdlib>

#include "ECS.h"
#include "Algorithm/Compare/CompareAlgorithmT.hpp"

namespace _GameEngine::_ECS
{
	bool SystemV2Key_comparator(SystemV2** p_left, SystemV2Key* p_right, void*)
	{
		return (*p_left)->SystemKey == *p_right;
	};

	bool SystemV2_comparator(SystemV2** left, SystemV2** right, void*)
	{
		return *left == *right;
	};
}

namespace _GameEngine::_ECS
{
	SystemV2* SystemV2_alloc(SystemV2AllocInfo* p_systemV2AllocInfo)
	{
		SystemV2* l_system = (SystemV2*)calloc(1, sizeof(SystemV2));
		l_system->ECS = p_systemV2AllocInfo->ECS;
		l_system->SystemKey = p_systemV2AllocInfo->SystemKey;
		EntityConfigurableContainer_init(&l_system->EntityConfigurableContainer, &p_systemV2AllocInfo->EntityConfigurableContainerInitInfo);
		l_system->Update = p_systemV2AllocInfo->Update;
		l_system->Child = p_systemV2AllocInfo->Child;
		l_system->OnSystemDestroyed = p_systemV2AllocInfo->OnSystemDestroyed;
		SystemContainer_addSystemV2(&l_system->ECS->SystemContainer, l_system);

		return l_system;
	};

	void SystemV2_free(SystemV2** p_systemV2)
	{
		SystemV2* l_system = *p_systemV2;
		EntityConfigurableContainer_free(&l_system->EntityConfigurableContainer, l_system->ECS);
		SystemContainer_removeSystemV2(&l_system->ECS->SystemContainer, l_system);

		if (l_system->OnSystemDestroyed)
		{
			l_system->OnSystemDestroyed(l_system);
		}
		free(l_system->Child);

		free(l_system);
		l_system = nullptr;
		p_systemV2 = nullptr;
	};

	void SystemContainer_addSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2)
	{
		_Core::VectorT_pushBack(&p_systemContainer->SystemsV2, &p_systemV2);
	};

	void SystemContainer_removeSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2)
	{
		_Core::VectorT_eraseCompare(&p_systemContainer->SystemsV2, _Core::ComparatorT<SystemV2*, SystemV2*, void>{ SystemV2_comparator, &p_systemV2 });
	};

	SystemV2* SystemContainer_getSystem(SystemContainer* p_systemContainer, SystemV2Key* p_key)
	{
		return *_Core::CompareT_find(_Core::VectorT_buildIterator(&p_systemContainer->SystemsV2), _Core::ComparatorT<SystemV2*, SystemV2Key, void>{SystemV2Key_comparator, p_key}).Current;
	};


	void SystemContainer_alloc(SystemContainer* p_systemContainer)
	{
		_Core::VectorT_alloc(&p_systemContainer->SystemsV2, 0);
	};

	void SystemContainer_free(SystemContainer* p_systemContainer)
	{
		_Core::VectorReverseIteratorT<SystemV2*> l_it = _Core::VectorT_buildReverseIterator(&p_systemContainer->SystemsV2);
		while (_Core::VectorReverseIteratorT_moveNext(&l_it))
		{
			SystemV2_free(l_it.Current);
		}
		_Core::VectorT_free(&p_systemContainer->SystemsV2);
	};
}