#include "System.h"

#include <cstdlib>
#include "Log/Log.h"

namespace _GameEngine::_ECS
{
	System* System_alloc(SystemAllocInfo* p_systemAllocInfo)
	{
		System* l_system = new System();
		l_system->_child = p_systemAllocInfo->Child;
		l_system->UpdateFunction = p_systemAllocInfo->UpdateFunction;
		l_system->OnSystemFree = p_systemAllocInfo->OnSystemFree;
		return l_system;
	};

	void System_free(System** p_system)
	{
		System* l_system = *p_system;

		if (l_system->OnSystemFree)
		{
			l_system->OnSystemFree(l_system);
		}

#ifndef NDEBUG
		if (l_system->_child)
		{
			_Log::LogInstance->CoreLogger->warn("Potential memory leak, System._child not freed.");
		}
#endif

		delete l_system;
		l_system = nullptr;
		p_system = nullptr;
	};

	void System_update(System* p_system, float p_delta)
	{
		p_system->UpdateFunction(p_system->_child, p_delta);
	};

	System* SystemContainer_allocSystem(SystemContainer* p_systemContainer, SystemAllocInfo* p_systemAllocInfo) 
	{
		System* p_instanciatedSystem = System_alloc(p_systemAllocInfo);
		p_systemContainer->Systems.emplace_back(p_instanciatedSystem);
		return p_instanciatedSystem;
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
		for (System* l_system : p_systemContainer->Systems)
		{
			System_free(&l_system);
		}

		p_systemContainer->Systems.clear();

		/*
		if (p_systemContainer->Systems.size() > 0)
		{
			// _Log::LogInstance->CoreLogger->warn("When the SystemContainer is being free");
		}
		*/
	};
}