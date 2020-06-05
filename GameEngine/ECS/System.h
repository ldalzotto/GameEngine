#pragma once

#include <vector>

namespace _GameEngine::_ECS
{
	struct SystemContainer;

	struct System
	{
		void* _child;
		void(*UpdateFunction)(void* p_child, float p_delta);
		void(*OnSystemFree)(void* p_child);
	};
	
	struct SystemAllocInfo
	{
		size_t ChildSize;
		void(*UpdateFunction)(void* p_child, float p_delta);
		void(*OnSystemFree)(void* p_child);
	};

	System* System_alloc(SystemAllocInfo* p_systemAllocInfo, SystemContainer* p_systemContainer);
	void System_free(System** p_system, SystemContainer* p_systemContainer);
	void System_update(System* p_system, float p_delta);

	struct SystemContainer
	{
		std::vector<System*> Systems;
	};

	void SystemContainer_update(SystemContainer* p_systemContainer, float p_delta);
	void SystemContainer_free(SystemContainer* p_systemContainer);
}