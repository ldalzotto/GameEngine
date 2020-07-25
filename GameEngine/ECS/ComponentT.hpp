#pragma once

#include "Component.h"

namespace _GameEngine::_ECS
{
	struct ECS;

	template <typename COMPONENT_TYPE>
	extern ComponentType* ComponentT_getComponentType();

	template <typename COMPONENT_TYPE>
	struct ComponentHeaderT
	{
		Entity* AttachedEntity;
		ComponentType* ComponentType;
		void(*OnComponentFree)(COMPONENT_TYPE*, ECS*);
	};

	template <typename COMPONENT_TYPE>
	COMPONENT_TYPE* ComponentT_alloc()
	{
		ComponentHeader* l_component = Component_alloc(ComponentT_getComponentType<COMPONENT_TYPE>(), sizeof(COMPONENT_TYPE));
		return (COMPONENT_TYPE*)l_component;
	};
}