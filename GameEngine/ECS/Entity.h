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
	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType);

	struct EntityContainer
	{
		std::vector<Entity*> Entities;
	};

	Entity* Entity_alloc(EntityContainer* p_entityContainer);
	void Entity_free(Entity** p_entity);

	void EntityContainer_free(EntityContainer* p_entityContainer);
};