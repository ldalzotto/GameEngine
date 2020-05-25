#include "ECS/System.h"

#include "Utils/Algorithm/Algorithm.h"
#include <vector>
#include "Entity.h"

namespace _GameEngine::_ECS
{

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component);
	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component);
	void registerComponentEventsForComponentTypes(EntityComponentListener* p_entityComponentListener, ComponentEvents* p_componentEvents);
	void pushEntityToEntityComponentListener(EntityComponentListener* l_entityComponentListener, Entity* p_entity);
	void pushEntityToEntityComponentListenerIfElligible(EntityComponentListener* l_entityComponentListener, Component* l_comparedComponent);
	void removeEntity(EntityComponentListener* p_entityComponentListener, Entity* p_entity);

	/**
		When initializing the @ref EntityComponentListener, we check the presence of the listened @ref ComponentType for all @ref Entity and push the
		Entity to the Listener if successfull.
		This is to be sure that all already present Entities are checked if the system initiation is done after the Entity creation.
	*/
	void pushAllElligibleEntitiesToEntityComponentListener(EntityComponentListener* l_entityComponentListener, std::vector<Entity*>* p_entities);


	void EntityComponentListener_init(EntityComponentListener* p_entityComponentListener, EntityComponentListenerInitInfo* p_entityComponentListenerInitInfo)
	{
		p_entityComponentListener->ListenedComponentTypes = p_entityComponentListenerInitInfo->ListenedComponentTypes;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesAdded = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesAdded;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesRemoved;

		p_entityComponentListener->ComponentAttachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->ComponentAttachedEventListener.Callback = entityComponentListener_onComponentAttachedCallback;

		p_entityComponentListener->ComponentDetachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->ComponentDetachedEventListener.Callback = entityComponentListener_onComponentDetachedCallback;

		pushAllElligibleEntitiesToEntityComponentListener(p_entityComponentListener, &p_entityComponentListenerInitInfo->EntityContainer->Entities);
		registerComponentEventsForComponentTypes(p_entityComponentListener, p_entityComponentListenerInitInfo->ComponentEvents);
	};

	void EntityComponentListener_free(EntityComponentListener* p_entityComponentListener, ComponentEvents* p_componentEvents)
	{
		for (ComponentType& l_componentType : p_entityComponentListener->ListenedComponentTypes)
		{
			_Utils::Observer_unRegister(&p_componentEvents->ComponentAttachedEvents[l_componentType], &p_entityComponentListener->ComponentAttachedEventListener);
			_Utils::Observer_unRegister(&p_componentEvents->ComponentDetachedEvents[l_componentType], &p_entityComponentListener->ComponentDetachedEventListener);
		}

		for (Entity*& l_entity : p_entityComponentListener->FilteredEntities)
		{
			removeEntity(p_entityComponentListener, l_entity);
		}

		p_entityComponentListener->FilteredEntities.clear();
	};

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityComponentListener* l_entityComponentListener = (EntityComponentListener*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;
		pushEntityToEntityComponentListenerIfElligible(l_entityComponentListener, l_component);
	};

	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityComponentListener* l_entityComponentListener = (EntityComponentListener*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;

		size_t l_foundIndex = _Utils::Vector_containsElementEquals(l_entityComponentListener->ListenedComponentTypes, l_component->ComponentType);
		if (l_foundIndex != -1)
		{
			removeEntity(l_entityComponentListener, l_component->AttachedEntity);
		}
	};

	void registerComponentEventsForComponentTypes(EntityComponentListener* p_entityComponentListener, ComponentEvents* p_componentEvents)
	{
		for (ComponentType& l_componentType : p_entityComponentListener->ListenedComponentTypes)
		{
			if (!p_componentEvents->ComponentAttachedEvents.contains(l_componentType))
			{
				p_componentEvents->ComponentAttachedEvents[l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&p_componentEvents->ComponentAttachedEvents[l_componentType], &p_entityComponentListener->ComponentAttachedEventListener);


			if (!p_componentEvents->ComponentDetachedEvents.contains(l_componentType))
			{
				p_componentEvents->ComponentDetachedEvents[l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&p_componentEvents->ComponentDetachedEvents[l_componentType], &p_entityComponentListener->ComponentDetachedEventListener);
		}
	};

	void pushEntityToEntityComponentListenerIfElligible(_GameEngine::_ECS::EntityComponentListener* l_entityComponentListener, _GameEngine::_ECS::Component* l_comparedComponent)
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
				pushEntityToEntityComponentListener(l_entityComponentListener, l_comparedComponent->AttachedEntity);
			}
		}
	};

	void pushAllElligibleEntitiesToEntityComponentListener(EntityComponentListener* l_entityComponentListener, std::vector<Entity*>* p_entities)
	{
		for (size_t i = 0; i < p_entities->size(); i++)
		{
			Entity*& p_entity = p_entities->at(i);
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

			pushEntityToEntityComponentListener(l_entityComponentListener, p_entity);

		checkNextEntity:;
		}
	};

	void pushEntityToEntityComponentListener(EntityComponentListener* l_entityComponentListener, Entity* p_entity)
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

	void removeEntity(EntityComponentListener* p_entityComponentListener, Entity* p_entity)
	{
		if (_Utils::Vector_eraseElementEquals(p_entityComponentListener->FilteredEntities, p_entity))
		{
			if (p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved)
			{
				p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved(p_entity);
			}
		}
	};
}