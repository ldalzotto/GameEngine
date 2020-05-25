#include "Entity.h"
#include "Utils/Algorithm/Algorithm.h"

namespace _GameEngine::_ECS
{
	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent, ComponentEvents* p_componentEvents)
	{
		p_entity->Components[p_unlinkedComponent->ComponentType] = p_unlinkedComponent;
		p_unlinkedComponent->AttachedEntity = p_entity;
		ComponentEvents_onComponentAttached(p_componentEvents, p_unlinkedComponent);
	};

	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType)
	{
		if (p_entity->Components.contains(p_componentType))
		{
			return p_entity->Components[p_componentType];
		}
		return nullptr;
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