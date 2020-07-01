#pragma once

#include <string>
#include <cstdlib>

#include <unordered_map>

#include "ComponentType.h"
#include "Utils/Observer/Observer.h"

namespace _GameEngine::_ECS
{
	struct Entity;

	struct Component
	{
		Entity* AttachedEntity;
		ComponentType ComponentType;
		void* Child;
		_Utils::Observer OnComponentFree;
	};

	Component* Component_alloc(ComponentType& p_type, size_t p_componentChildSize);

	template<class COMPONENT_TYPE>
	inline Component* Component_alloc(ComponentType& p_type, COMPONENT_TYPE** out_componentChild)
	{
		Component* l_instanciatedCOmponent = Component_alloc(p_type, sizeof(COMPONENT_TYPE));
		if (out_componentChild)
		{
			*out_componentChild = (COMPONENT_TYPE*)l_instanciatedCOmponent->Child;
		}
		return l_instanciatedCOmponent;
	};

	void Component_free(Component** p_component);

	bool Component_comparator(Component** left, ComponentType* right);

	struct ComponentEvents
	{
		/** Triggered when any Component with  the specified ComponentType is attached to an Entity. */
		std::unordered_map<ComponentType, _Utils::Observer> ComponentAttachedEvents;
		/** Triggered when any Component with  the specified ComponentType is dettached to an Entity. */
		std::unordered_map<ComponentType, _Utils::Observer> ComponentDetachedEvents;
	};

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, Component* p_component);
	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, Component* p_component);

};