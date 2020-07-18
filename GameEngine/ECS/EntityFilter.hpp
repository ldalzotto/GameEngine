#pragma once

#include "Functional/Callback/CallbackT.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"
#include "ECS/ComponentType.h"

namespace _GameEngine::_ECS
{
	struct ECS;
	struct Component;
	struct Entity;
}

namespace _GameEngine::_ECS
{
	struct EntityFilter
	{
		_Core::ArrayT<ComponentType> ListenedComponentTypes;

		/** This callback is called when the components listed in ListenedComponentTypes are attached to the Entity.
			It is called after component initialization. */
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesAdded;

		/** This callback is called when the components liste in the ListenedComponentTypes are no more attached to the Entity.
			It is called before the components are freed. */
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesRemoved;
	};

	void EntityFilter_alloc_1c(EntityFilter* p_entityFiler, ECS* p_ecs, 
			ComponentType* l_listenedType1, 
		void* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(void*, Entity*), void(*p_OnEntityThatMatchesComponentTypesRemoved)(void*, Entity*));

	void EntityFilter_alloc_2c(EntityFilter* p_entityFiler, ECS* p_ecs, 
		ComponentType* l_listenedType1, ComponentType* l_listenedType2,
		void* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(void*, Entity*), void(*p_OnEntityThatMatchesComponentTypesRemoved)(void*, Entity*));

	void EntityFilter_free(EntityFilter* p_entityFiler, ECS* p_ecs);

}