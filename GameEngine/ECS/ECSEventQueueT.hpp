#pragma once

#include "ECSEventQueue.h"

namespace _GameEngine::_ECS
{
	namespace _GameEngine::_ECS
	{
		struct ECS;
		struct Entity;
		struct Component;
	}

	template <typename COMPONENT_TYPE>
	ECSEventMessage* ECSEventMessageT_AddComponent_alloc(Entity** l_entity, COMPONENT_TYPE** l_allocatedComponent)
	{
		return ECSEventMessage_AddComponent_alloc(l_entity, &((Component*)l_allocatedComponent));
	};
}