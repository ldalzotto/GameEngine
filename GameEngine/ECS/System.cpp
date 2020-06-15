#include "System.h"

#include <cstdlib>
#include "Log/Log.h"

#include "ECS.h"

namespace _GameEngine::_ECS
{
	SystemV2* SystemV2_alloc(SystemV2AllocInfo* p_systemV2AllocInfo)
	{
		SystemV2* l_system = (SystemV2*)calloc(1, sizeof(SystemV2));
		l_system->ECS = p_systemV2AllocInfo->ECS;
		EntityConfigurableContainer_init(&l_system->EntityConfigurableContainer, &p_systemV2AllocInfo->EntityConfigurableContainerInitInfo);
		l_system->Update = p_systemV2AllocInfo->Update;
		SystemContainer_addSystemV2(&l_system->ECS->SystemContainer, l_system);
		
		return l_system;
	};

	void SystemV2_free(SystemV2** p_systemV2)
	{
		SystemV2* l_system = *p_systemV2;
		EntityConfigurableContainer_free(&l_system->EntityConfigurableContainer, l_system->ECS);
		SystemContainer_removeSystemV2(&l_system->ECS->SystemContainer, l_system);
		free(l_system);
		l_system = nullptr;
		p_systemV2 = nullptr;
	};

	bool SystemV2_comparator(SystemV2** left, SystemV2** right)
	{
		return *left == *right;
	};

	void SystemContainer_addSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2)
	{
		p_systemContainer->SystemsV2.push_back(&p_systemV2);
	};

	void SystemContainer_removeSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2)
	{
		p_systemContainer->SystemsV2.erase(SystemV2_comparator, &p_systemV2);
	};

	void system_reverseLoop_erase(SystemV2** p_system, void* null)
	{
		SystemV2_free(p_system);
	}


	void SystemContainer_alloc(SystemContainer* p_systemContainer)
	{
		p_systemContainer->SystemsV2.alloc();
	};

	void SystemContainer_free(SystemContainer* p_systemContainer)
	{
		p_systemContainer->SystemsV2.forEachReverse(system_reverseLoop_erase, (void*)nullptr);
		p_systemContainer->SystemsV2.free();
	};
}