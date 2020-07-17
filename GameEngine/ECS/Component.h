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

	struct Component;

	typedef void(*OnComponentFunction)(Component*);

	struct Component
	{
		void* Child;
		Entity* AttachedEntity;
		ComponentType* ComponentType;

		OnComponentFunction OnComponentFree;
	};

	Component* Component_alloc(ComponentType* p_type, size_t p_componentChildSize);

	void Component_free(Component** p_component);

	bool Component_comparator(Component** left, ComponentType* right, void*);

	struct ComponentEvents
	{
		/** Triggered when any Component with  the specified ComponentType is attached to an Entity. */
		std::unordered_map<ComponentType, _Core::ObserverT<Component>> ComponentAttachedEvents;
		/** Triggered when any Component with  the specified ComponentType is dettached to an Entity. */
		std::unordered_map<ComponentType, _Core::ObserverT<Component>> ComponentDetachedEvents;
	};

	void ComponentEvents_free(ComponentEvents* p_componentEvents, ECS* p_ecs);

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, Component* p_component);
	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, Component* p_component);

};