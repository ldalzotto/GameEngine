#pragma once

#include <string>
#include <cstdlib>

#include <unordered_map>

#include "ComponentType.h"

#include "Functional/Callback/ObserverT.hpp"

namespace _GameEngine::_ECS
{
	struct Entity;
	struct ECS;

	struct ComponentHeader;

	typedef void(*OnComponentFunction)(ComponentHeader*, ECS*);

	struct ComponentHeader
	{
		Entity* AttachedEntity;
		ComponentType* ComponentType;
		OnComponentFunction OnComponentFree;
	};

	bool Component_comparator(ComponentHeader** left, ComponentType* right, void*);

	ComponentHeader* Component_alloc(ComponentType* p_type, size_t p_componentSize);

	void Component_free(ComponentHeader** p_component, ECS* p_ecs);

	struct ComponentEvents
	{
		/** Triggered when any Component with  the specified ComponentType is attached to an Entity. */
		std::unordered_map<ComponentType, _Core::ObserverT<ComponentHeader>> ComponentAttachedEvents;
		/** Triggered when any Component with  the specified ComponentType is dettached to an Entity. */
		std::unordered_map<ComponentType, _Core::ObserverT<ComponentHeader>> ComponentDetachedEvents;
	};

	void ComponentEvents_free(ComponentEvents* p_componentEvents, ECS* p_ecs);

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, ComponentHeader* p_component);
	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, ComponentHeader* p_component);

};