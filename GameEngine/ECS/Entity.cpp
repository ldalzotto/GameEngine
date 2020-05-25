#include "Entity.h"
#include "Utils/Algorithm/Algorithm.h"

namespace _GameEngine::_ECS
{

	Entity* entity_alloc();
	void entity_free(Entity** p_entity, ComponentEvents* p_componentEvents);

	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent, ComponentEvents* p_componentEvents)
	{
		p_entity->Components[p_unlinkedComponent->ComponentType] = p_unlinkedComponent;
		p_unlinkedComponent->AttachedEntity = p_entity;
		ComponentEvents_onComponentAttached(p_componentEvents, p_unlinkedComponent);
	};

	void Entity_freeComponent(Entity* p_entity, Component** p_component, ComponentEvents* p_componentEvents)
	{
		ComponentEvents_onComponentDetached(p_componentEvents, (*p_component));
		p_entity->Components.erase((*p_component)->ComponentType);
		Component_free(p_component);
	};

	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType)
	{
		if (p_entity->Components.contains(p_componentType))
		{
			return p_entity->Components[p_componentType];
		}
		return nullptr;
	};


	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents)
	{
		for (size_t i = p_entityContainer->Entities.size(); i--;)
		{
			EntityContainer_freeEntity(&p_entityContainer->Entities[i], nullptr, p_componentEvents);
		}

		p_entityContainer->Entities.clear();
	};

	Entity* EntityContainer_allocEntity(EntityContainer* p_entityContainer)
	{
		Entity* l_instanciatedEntity = entity_alloc();
		p_entityContainer->Entities.emplace_back(l_instanciatedEntity);
		return l_instanciatedEntity;
	};

	void EntityContainer_freeEntity(Entity** p_entity, EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents)
	{
		if (p_entityContainer)
		{
			_Utils::Vector_eraseElementEquals(p_entityContainer->Entities, (*p_entity));
		}

		entity_free(p_entity, p_componentEvents);
	};

	Entity* entity_alloc()
	{
		Entity* l_instanciatedEntity = new Entity();
		return l_instanciatedEntity;
	};

	void entity_free(Entity** p_entity, ComponentEvents* p_componentEvents)
	{
		auto l_copiedComponents = (*p_entity)->Components;

		for (auto&& p_component : l_copiedComponents)
		{
			Entity_freeComponent((*p_entity), &p_component.second, p_componentEvents);
		}
		delete (*p_entity);
		*p_entity = nullptr;
	};

};