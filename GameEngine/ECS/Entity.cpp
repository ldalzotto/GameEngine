#include "Entity.h"
#include "Utils/Algorithm/Algorithm.h"

namespace _GameEngine::_ECS
{
	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent)
	{
		p_entity->Components[p_unlinkedComponent->ComponentType] = p_unlinkedComponent;
		p_unlinkedComponent->AttachedEntity = p_entity;
	};

	Entity* Entity_alloc(EntityContainer* p_entityContainer)
	{
		Entity* l_instanciatedEntity = new Entity();
		p_entityContainer->Entities.push_back(l_instanciatedEntity);
		return l_instanciatedEntity;
	};

	void Entity_free(Entity** p_entity)
	{
		for (auto&& p_component : (*p_entity)->Components)
		{
			Component_free(&p_component.second);
		}
		delete (*p_entity);
	};

	void EntityContainer_free(EntityContainer* p_entityContainer)
	{
		for (size_t i = p_entityContainer->Entities.size(); i--;)
		{
			Entity_free(&p_entityContainer->Entities[i]);
		}

		p_entityContainer->Entities.clear();
	};

};