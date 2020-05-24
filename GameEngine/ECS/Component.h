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

	void Component_init(Component* p_component, ComponentType p_cType, size_t p_childSize);
	void Component_free(Component* p_component);

#define BUILD_COMPONENT(cName, cType) _GameEngine::_ECS::Component{ cType, malloc(sizeof(cName)) }


};