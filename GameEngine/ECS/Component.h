#pragma once

#include <string>
#include <cstdlib>

namespace _GameEngine::_ECS
{
	typedef std::string ComponentType;

	struct Component
	{
		ComponentType ComponentType;
		void* Child;
		void(*Component_freeCallback)(Component* p_component);
	};

	void Component_free(Component* p_component);
	Component Component_Build(ComponentType p_type, void* p_child);

};