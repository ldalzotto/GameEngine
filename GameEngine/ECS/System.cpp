#include "System.h"

#include <cstdlib>
#include "Log/Log.h"

#include "Utils/Algorithm/Algorithm.h"

namespace _GameEngine::_ECS
{
	System* System_alloc(SystemAllocInfo* p_systemAllocInfo, SystemContainer* p_systemContainer)
	{
		System* l_system = new System();
		l_system->_child = calloc(1, p_systemAllocInfo->ChildSize);
		l_system->UpdateFunction = p_systemAllocInfo->UpdateFunction;
		p_systemContainer->Systems.emplace_back(l_system);
		return l_system;
	};

	void System_free(System** p_system, SystemContainer* p_systemContainer)
	{
		System* l_system = *p_system;

		_Utils::Vector_eraseElementEquals(&p_systemContainer->Systems, l_system);

		if (l_system->OnSystemFree)
		{
			l_system->OnSystemFree(l_system->_child);
		}
		free(l_system->_child);
		delete l_system;
		l_system = nullptr;
		p_system = nullptr;
	};

	void System_update(System* p_system, float p_delta)
	{
		p_system->UpdateFunction(p_system->_child, p_delta);
	};

	void SystemContainer_update(SystemContainer* p_systemContainer, float p_delta)
	{
		for (System* l_system : p_systemContainer->Systems)
		{
			System_update(l_system, p_delta);
		}
	};

	void SystemContainer_free(SystemContainer* p_systemContainer)
	{
		if (p_systemContainer->Systems.size() > 0)
		{
			_Log::LogInstance->CoreLogger->warn("When the SystemContainer is being free");
		}
	};
}