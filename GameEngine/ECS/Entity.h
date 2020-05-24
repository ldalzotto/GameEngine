#pragma once

#include <unordered_map>
#include <list>

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{

	struct Entity
	{
		std::unordered_map<ComponentType, Component> Components;
	};

	Component* Entity_addComponent(Entity* p_entity, Component& p_unlinkedComponent);
	void Entity_free(Entity* p_entity);

	struct EntityContainer
	{
		std::vector<Entity> Entities;
	};

	void EntityContainer_pushEntity(EntityContainer* p_entityContainer, Entity* p_entity);
	void EntityContainer_free(EntityContainer* p_entityContainer);
};