#pragma once

#include <vector>

namespace _GameEngine::_ECS
{
	struct System
	{
		void* _child;
		void(*OnSystemFree)(System* p_system);
	};
	
	struct SystemAllocInfo
	{
		void* Child;
		void(*OnSystemFree)(System* p_system);
	};

	System* System_alloc(SystemAllocInfo* p_systemAllocInfo);
	void System_free(System** p_system);

	struct SystemContainer
	{
		std::vector<System*> Systems;
	};

	System* SystemContainer_allocSystem(SystemContainer* p_systemContainer, SystemAllocInfo* p_systemAllocInfo);
	void SystemContainer_free(SystemContainer* p_systemContainer);
}