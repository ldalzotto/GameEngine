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

	void SystemContainer_addSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2)
	{
		p_systemContainer->SystemsV2.emplace_back(p_systemV2);
	};

	void SystemContainer_removeSystemV2(SystemContainer* p_systemContainer, SystemV2* p_systemV2)
	{
		for (size_t i = 0; i < p_systemContainer->SystemsV2.size(); i++)
		{
			SystemV2* l_systemV2 = p_systemContainer->SystemsV2.at(i);
			if (l_systemV2 == p_systemV2)
			{
				p_systemContainer->SystemsV2.erase(p_systemContainer->SystemsV2.begin() + i);
				break;
			}
		}
	};

	void SystemContainer_free(SystemContainer* p_systemContainer)
	{
		std::vector<SystemV2*> l_systemV2s = std::vector<SystemV2*>(p_systemContainer->SystemsV2);
		for (size_t i = 0; i < l_systemV2s.size(); i++)
		{
			SystemV2_free(&l_systemV2s.at(i));
		}
	};
}