#include "Entity.h"

#include <stdexcept>

#include "ECS/ECS.h"
#include "Utils/Algorithm/Algorithm.h"
#include "Log/Log.h"

namespace _GameEngine::_ECS
{

	Entity* entity_alloc(ECS* p_ecs);
	void entity_free(Entity** p_entity);

	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent)
	{

#ifndef NDEBUG
		if (p_entity->Components.contains(p_unlinkedComponent->ComponentType))
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Trying to add a component were it's type ( " + p_unlinkedComponent->ComponentType + " ) is aleady present as a component."));
		}
#endif

		p_entity->Components[p_unlinkedComponent->ComponentType] = p_unlinkedComponent;
		p_unlinkedComponent->AttachedEntity = p_entity;
		ComponentEvents_onComponentAttached(&p_entity->ECS->ComponentEvents, p_unlinkedComponent);
	};

	void Entity_freeComponent(Entity* p_entity, Component** p_component)
	{
		ComponentEvents_onComponentDetached(&p_entity->ECS->ComponentEvents, (*p_component));
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

	Entity* EntityContainer_allocEntity(ECS* p_ecs)
	{
		Entity* l_instanciatedEntity = entity_alloc(p_ecs);
		p_ecs->EntityContainer.Entities.emplace_back(l_instanciatedEntity);
		return l_instanciatedEntity;
	};

	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents)
	{
		// We copy entities vector because operations inside the loop writes to the initial array
		std::vector<Entity*> l_copiedEntitiesPointer = p_entityContainer->Entities;
		for (size_t i = l_copiedEntitiesPointer.size(); i--;)
		{
			EntityContainer_freeEntity(&l_copiedEntitiesPointer[i]);
		}

		p_entityContainer->Entities.clear();
	};

	void EntityContainer_freeEntity(Entity** p_entity)
	{
		_Utils::Vector_eraseElementEquals((*p_entity)->ECS->EntityContainer.Entities, (*p_entity));
		entity_free(p_entity);
	};

	Entity* entity_alloc(ECS* p_ecs)
	{
		Entity* l_instanciatedEntity = new Entity();
		l_instanciatedEntity->ECS = p_ecs;
		return l_instanciatedEntity;
	};

	void entity_free(Entity** p_entity)
	{
		// We copy components vector because operations inside the loop writes to the initial array
		auto l_copiedComponents = (*p_entity)->Components;
		for (auto&& p_component : l_copiedComponents)
		{
			Entity_freeComponent((*p_entity), &p_component.second);
		}
		delete (*p_entity);
		*p_entity = nullptr;
	};

	///////////////////////////////////
	///////////////////////////////////  EntityConfigurableContainer
	///////////////////////////////////

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component);
	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component);
	void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
	void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity);
	void entityComponentListener_pushEntityToEntityIfElligible(EntityConfigurableContainer* l_entityComponentListener, Component* l_comparedComponent);
	void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity);

	/**
		When initializing the @ref EntityConfigurableContainer, we check the presence of the listened @ref ComponentType for all @ref Entity and push the
		Entity to the Listener if successfull.
		This is to be sure that all already present Entities are checked if the system initiation is done after the Entity creation.
	*/
	void entityComponentListener_pushAllElligibleEntities(EntityConfigurableContainer* l_entityComponentListener, std::vector<Entity*>* p_queriedEntities);


	void EntityConfigurableContainer_init(EntityConfigurableContainer* p_entityComponentListener, EntityConfigurableContainerInitInfo* p_entityComponentListenerInitInfo)
	{
		p_entityComponentListener->ListenedComponentTypes = p_entityComponentListenerInitInfo->ListenedComponentTypes;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesAdded = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesAdded;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesRemoved;

		p_entityComponentListener->OnComponentAttachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->OnComponentAttachedEventListener.Callback = entityComponentListener_onComponentAttachedCallback;

		p_entityComponentListener->OnComponentDetachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->OnComponentDetachedEventListener.Callback = entityComponentListener_onComponentDetachedCallback;

		entityComponentListener_pushAllElligibleEntities(p_entityComponentListener, &p_entityComponentListenerInitInfo->ECS->EntityContainer.Entities);
		entityComponentListener_registerEvents(p_entityComponentListener, p_entityComponentListenerInitInfo->ECS);
	};

	void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;
		for (ComponentType& l_componentType : p_entityComponentListener->ListenedComponentTypes)
		{
			_Utils::Observer_unRegister(&l_componentEvents->ComponentAttachedEvents[l_componentType], &p_entityComponentListener->OnComponentAttachedEventListener);
			_Utils::Observer_unRegister(&l_componentEvents->ComponentDetachedEvents[l_componentType], &p_entityComponentListener->OnComponentDetachedEventListener);
		}

		for (Entity*& l_entity : p_entityComponentListener->FilteredEntities)
		{
			entityComponentListener_removeEntity(p_entityComponentListener, l_entity);
		}

		p_entityComponentListener->FilteredEntities.clear();
	};

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityConfigurableContainer* l_entityComponentListener = (EntityConfigurableContainer*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;
		entityComponentListener_pushEntityToEntityIfElligible(l_entityComponentListener, l_component);
	};

	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityConfigurableContainer* l_entityComponentListener = (EntityConfigurableContainer*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;

		size_t l_foundIndex = _Utils::Vector_containsElementEquals(l_entityComponentListener->ListenedComponentTypes, l_component->ComponentType);
		if (l_foundIndex != -1)
		{
			entityComponentListener_removeEntity(l_entityComponentListener, l_component->AttachedEntity);
		}
	};

	void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		for (ComponentType& l_componentType : p_entityComponentListener->ListenedComponentTypes)
		{
			if (!l_componentEvents->ComponentAttachedEvents.contains(l_componentType))
			{
				l_componentEvents->ComponentAttachedEvents[l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&l_componentEvents->ComponentAttachedEvents[l_componentType], &p_entityComponentListener->OnComponentAttachedEventListener);


			if (!l_componentEvents->ComponentDetachedEvents.contains(l_componentType))
			{
				l_componentEvents->ComponentDetachedEvents[l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&l_componentEvents->ComponentDetachedEvents[l_componentType], &p_entityComponentListener->OnComponentDetachedEventListener);
		}
	};

	void entityComponentListener_pushEntityToEntityIfElligible(_GameEngine::_ECS::EntityConfigurableContainer* l_entityComponentListener, _GameEngine::_ECS::Component* l_comparedComponent)
	{
		if (_Utils::Vector_containsElementEquals(l_entityComponentListener->ListenedComponentTypes, l_comparedComponent->ComponentType) != -1)
		{
			bool l_addEntity = true;
			for (ComponentType& l_componentType : l_entityComponentListener->ListenedComponentTypes)
			{
				bool l_filteredComponentTypeIsAnEntityComponent = false;
				for (auto&& l_entityComponentEntry : l_comparedComponent->AttachedEntity->Components)
				{
					if (l_entityComponentEntry.first == l_componentType)
					{
						l_filteredComponentTypeIsAnEntityComponent = true;
						break;
					}
				}

				if (!l_filteredComponentTypeIsAnEntityComponent)
				{
					l_addEntity = false;
					break;
				}
			}


			if (l_addEntity)
			{
				entityComponentListener_pushEntity(l_entityComponentListener, l_comparedComponent->AttachedEntity);
			}
		}
	};

	void entityComponentListener_pushAllElligibleEntities(EntityConfigurableContainer* l_entityComponentListener, std::vector<Entity*>* p_queriedEntities)
	{
		for (size_t i = 0; i < p_queriedEntities->size(); i++)
		{
			Entity*& p_entity = p_queriedEntities->at(i);
			for (ComponentType& l_askedComponentType : l_entityComponentListener->ListenedComponentTypes)
			{
				bool l_componentTypeMatchFound = false;
				for (auto&& l_componentEntry : p_entity->Components)
				{
					if (l_askedComponentType == l_componentEntry.first)
					{
						l_componentTypeMatchFound = true;
						break;
					}
				}

				if (!l_componentTypeMatchFound)
				{
					goto checkNextEntity;
				}
			}

			entityComponentListener_pushEntity(l_entityComponentListener, p_entity);

		checkNextEntity:;
		}
	};

	void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity)
	{
		if (_Utils::Vector_containsElementEquals(l_entityComponentListener->FilteredEntities, p_entity) == -1)
		{
			l_entityComponentListener->FilteredEntities.emplace_back(p_entity);
		}

		if (l_entityComponentListener->OnEntityThatMatchesComponentTypesAdded)
		{
			l_entityComponentListener->OnEntityThatMatchesComponentTypesAdded(p_entity);
		}
	};

	void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity)
	{
		if (_Utils::Vector_eraseElementEquals(p_entityComponentListener->FilteredEntities, p_entity))
		{
			if (p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved)
			{
				p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved(p_entity);
			}
		}
	};


};