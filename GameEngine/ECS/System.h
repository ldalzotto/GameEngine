#pragma once

#include <vector>

namespace _GameEngine::_ECS
{
	struct System
	{
		void* _child;
		void(*UpdateFunction)(void* p_child, float p_delta);
		void(*OnSystemFree)(System* p_system);
	};
	
	struct SystemAllocInfo
	{
		void* Child;
		void(*UpdateFunction)(void* p_child, float p_delta);
		void(*OnSystemFree)(System* p_system);
	};

	System* System_alloc(SystemAllocInfo* p_systemAllocInfo);
	void System_free(System** p_system);
	void System_update(System* p_system, float p_delta);

	struct SystemContainer
	{
		std::vector<System*> Systems;
	};

	System* SystemContainer_allocSystem(SystemContainer* p_systemContainer, SystemAllocInfo* p_systemAllocInfo);
	void SystemContainer_update(SystemContainer* p_systemContainer, float p_delta);
	void SystemContainer_free(SystemContainer* p_systemContainer);
}