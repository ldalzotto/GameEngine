#pragma once

#include <string>
#include <cstdlib>

#include <unordered_map>

#include "Utils/Observer/Observer.h"

namespace _GameEngine::_ECS
{
	struct Entity;

	typedef std::string ComponentType;

	struct Component
	{
		Entity* AttachedEntity;
		ComponentType ComponentType;
		void* Child;
		void(*Component_freeCallback)(Component* p_component);
	};

	Component* Component_alloc(ComponentType& p_type, void* p_child);
	void Component_free(Component** p_component);

	/**
		@ref _Utils::Observer closure is nullptr
	*/
	struct ComponentEvents
	{
		std::unordered_map<ComponentType, _Utils::Observer> ComponentAttachedEvents;
		std::unordered_map<ComponentType, _Utils::Observer> ComponentDetachedEvents;
	};

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, Component* p_component);
	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, Component* p_component);

};