#pragma once

#include "Entity.h"

namespace _GameEngine::_ECS
{
	template <typename COMPONENT_TYPE>
	inline void EntityT_addComponentDeferred(Entity* p_entity, COMPONENT_TYPE** p_unlinkedComponent, ECS* p_ecs)
	{
		Entity_addComponentDeferred(p_entity, (Component*)p_unlinkedComponent, p_ecs);
	};

	template <typename COMPONENT_TYPE>
	inline COMPONENT_TYPE** EntityT_getComponent(Entity* p_entity)
	{
		return (COMPONENT_TYPE**)Entity_getComponent(p_entity, ComponentT_getComponentType<COMPONENT_TYPE>());
	};
}