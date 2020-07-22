#pragma once

#include "DataStructures/Specifications/VectorT.hpp"

#include "ECS/Component.h"

#include "Functional/Callback/CallbackT.hpp"


namespace _GameEngine::_ECS
{
	struct ECS;

	struct Entity
	{
		_Core::VectorT<ComponentHeader*> Components;
	};

	bool Entity_comparator(Entity** p_left, Entity** p_right, void*);

	Entity* Entity_alloc();

	void Entity_addComponentDeferred(Entity* p_entity, ComponentHeader* p_unlinkedComponent, ECS* p_ecs);
	void Entity_addComponent(Entity* p_entity, ComponentHeader* p_unlinkedComponent, ECS* p_ecs);
	void Entity_freeComponent(Entity* p_entity, ComponentHeader** p_component, ECS* p_ecs);

	ComponentHeader* Entity_getComponent(Entity* p_entity, ComponentType* p_componentType);

	struct EntityContainer
	{
		ECS* ECS;
		_Core::VectorT<Entity*> Entities;
	};

	void EntityContainer_freeEntity(EntityContainer* p_entityContainer, Entity** p_entity);
	void EntityContainer_alloc(EntityContainer* p_entityContainer, ECS* p_ecs);
	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents);

	void EntityContainer_sendEventToDeleteAllEntities(EntityContainer* p_entityContainer);

	///////////////////////////////////
	///////////////////////////////////  EntityConfigurableContainer
	///////////////////////////////////

	struct EntityConfigurableContainer
	{
		_Core::VectorT<ComponentType> ListenedComponentTypes;
		_Core::VectorT<Entity*> FilteredEntities;

		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesAdded;
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesRemoved;
	};

	struct EntityConfigurableContainerInitInfo
	{
		_Core::VectorT<ComponentType> ListenedComponentTypes;

		/** This callback is called when the components listed in ListenedComponentTypes are attached to the Entity.
			It is called after component initialization. */
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesAdded;

		/** This callback is called when the components liste in the ListenedComponentTypes are no more attached to the Entity.
			It is called before the components are freed. */
		_Core::CallbackT<void, Entity> OnEntityThatMatchesComponentTypesRemoved;

		ECS* ECS;
	};

	void EntityConfigurableContainer_init(EntityConfigurableContainer* p_entityComponentListener, EntityConfigurableContainerInitInfo* p_entityComponentListenerInitInfo);
	void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
};