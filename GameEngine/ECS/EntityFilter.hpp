#pragma once

#include "Functional/Callback/CallbackT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
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
		_Core::VectorT<ComponentType> ListenedComponentTypes;

		/** This callback is called when the components listed in ListenedComponentTypes are attached to the Entity.
			It is called after component initialization. */
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesAdded;

		/** This callback is called when the components liste in the ListenedComponentTypes are no more attached to the Entity.
			It is called before the components are freed. */
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesRemoved;
	};

	void EntityFilter_init(EntityFilter* p_entityFiler, ECS* p_ecs);
	void EntityFilter_free(EntityFilter* p_entityFiler, ECS* p_ecs);

}