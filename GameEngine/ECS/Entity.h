#pragma once

#include <unordered_map>
#include <list>

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{

	struct Entity
	{
		std::unordered_map<ComponentType, Component*> Components;
	};

	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent, ComponentEvents* p_componentEvents);
	void Entity_freeComponent(Entity* p_entity, Component** p_component, ComponentEvents* p_componentEvents);
	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType);

	struct EntityContainer
	{
		std::vector<Entity*> Entities;
	};

	Entity* EntityContainer_allocEntity(EntityContainer* p_entityContainer);
	void EntityContainer_freeEntity(Entity** p_entity, EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents);
	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents);
};