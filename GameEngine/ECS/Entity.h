#pragma once

#include <unordered_map>
#include <list>

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	struct ECS;

	struct Entity
	{
		ECS* ECS;
		std::unordered_map<ComponentType, Component*> Components;
	};
	 
	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent);
	void Entity_freeComponent(Entity* p_entity, Component** p_component);

	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType);

#define GET_COMPONENT(ComponentTypeName, EntityPointer) (ComponentTypeName*) Entity_getComponent(EntityPointer, #ComponentTypeName)->Child

	struct EntityContainer
	{
		std::vector<Entity*> Entities;
	};

	Entity* EntityContainer_allocEntity(ECS* p_ecs);
	void EntityContainer_freeEntity(Entity** p_entity);
	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents);

	///////////////////////////////////
	///////////////////////////////////  EntityConfigurableContainer
	///////////////////////////////////

	struct EntityConfigurableContainer
	{
		std::vector<ComponentType> ListenedComponentTypes;
		std::vector<Entity*> FilteredEntities;
		void(*OnEntityThatMatchesComponentTypesAdded)(Entity*);
		void(*OnEntityThatMatchesComponentTypesRemoved)(Entity*);

		/**
			Called when a @ref Component with type contained in @ref ListenedComponentTypes is attached.
		*/
		_Utils::Subject OnComponentAttachedEventListener;

		/**
			Called when a @ref Component with type contained in @ref ListenedComponentTypes is detached.
		*/
		_Utils::Subject OnComponentDetachedEventListener;
	};

	struct EntityConfigurableContainerInitInfo
	{
		std::vector<ComponentType> ListenedComponentTypes;
		void(*OnEntityThatMatchesComponentTypesAdded)(Entity*);
		void(*OnEntityThatMatchesComponentTypesRemoved)(Entity*);

		ECS* ECS;
	};

	void EntityConfigurableContainer_init(EntityConfigurableContainer* p_entityComponentListener, EntityConfigurableContainerInitInfo* p_entityComponentListenerInitInfo);
	void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
};