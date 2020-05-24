#include "Entity.h"

namespace _GameEngine::_ECS
{
	Component* Entity_addComponent(Entity* p_entity, Component& p_unlinkedComponent)
	{
		p_entity->Components[p_unlinkedComponent.ComponentType] = p_unlinkedComponent;
		return &p_entity->Components.at(p_unlinkedComponent.ComponentType);
	};

	void Entity_free(Entity* p_entity)
	{
		for (auto&& p_component : p_entity->Components)
		{
			Component_free(&p_component.second);
		}
	};

	void EntityContainer_pushEntity(EntityContainer* p_entityContainer, Entity* p_entity)
	{
		p_entityContainer->Entities.emplace_back(*p_entity);
	};

	void EntityContainer_free(EntityContainer* p_entityContainer)
	{
		for (size_t i = p_entityContainer->Entities.size(); i--;)
		{
			Entity_free(&p_entityContainer->Entities[i]);
		}

		p_entityContainer->Entities.clear();
	};

	void EntityContainer_udpate(EntityContainer* p_entityContainer, float p_delta)
	{
	}
	;
};