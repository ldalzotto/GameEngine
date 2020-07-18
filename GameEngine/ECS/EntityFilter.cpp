#include "EntityFilter.hpp"

#include "Algorithm/Compare/CompareAlgorithmT.hpp"
#include "ECS/ECS.h"
#include "ECS/ComponentType.h"

namespace _GameEngine::_ECS
{
	void entityFilter_registerEvents(EntityFilter* p_entityFilter, ECS* p_ecs);
	void entityFilter_unregisterEvents(EntityFilter* p_entityFilter, ECS* p_ecs);

	void entityFilter_notifyIfEntityMatches(EntityFilter* p_entityFiler, Entity* p_entity);
	void entityFilter_notifyIfJustUnmatched(EntityFilter* p_entityFiler, Entity* p_entity);

	/**
	When initializing the @ref EntityFilter, we check the presence of the listened @ref ComponentType for all @ref Entity and push the
	Entity to the Listener if successfull.
	This is to be sure that all already present Entities are checked if the system initiation is done after the Entity creation.
	*/
	void entityFilter_notifyAllMatchedEntities(EntityFilter* p_entityFiler, _Core::VectorT<Entity*>* p_queriedEntities);

	void EntityFilter_init(EntityFilter* p_entityFiler, ECS* p_ecs)
	{
		entityFilter_notifyAllMatchedEntities(p_entityFiler, &p_ecs->EntityContainer.Entities);
		entityFilter_registerEvents(p_entityFiler, p_ecs);
	};
	
	void EntityFilter_free(EntityFilter* p_entityFiler, ECS* p_ecs)
	{
		entityFilter_unregisterEvents(p_entityFiler, p_ecs);
		_Core::VectorT_free(&p_entityFiler->ListenedComponentTypes);
	};

	void entityFilter_notifyAllMatchedEntities(EntityFilter* p_entityFiler, _Core::VectorT<Entity*>* p_queriedEntities)
	{
		_Core::VectorIteratorT<Entity*> l_entity = _Core::VectorT_buildIterator(p_queriedEntities);
		while (_Core::VectorIteratorT_moveNext(&l_entity))
		{
			entityFilter_notifyIfEntityMatches(p_entityFiler, *l_entity.Current);
		}
	};

	void entityFilter_notifyIfEntityMatches(EntityFilter* p_entityFiler, Entity* p_entity)
	{
		bool l_notify = true;

		_Core::VectorIteratorT<ComponentType> l_filteredComponentType = _Core::VectorT_buildIterator(&p_entityFiler->ListenedComponentTypes);
		while (_Core::VectorIteratorT_moveNext(&l_filteredComponentType))
		{
			if (!_Core::CompareT_contains(_Core::VectorT_buildIterator(&p_entity->Components), _Core::ComparatorT<Component*, ComponentType, void>{Component_comparator, l_filteredComponentType.Current}))
			{
				l_notify = false;
				break;
			};
		}

		if (l_notify)
		{
			_Core::CallbackT_call(&p_entityFiler->OnEntityThatMatchesComponentTypesAdded, p_entity);
		}
	};

	void entityFilter_notifyIfJustUnmatched(EntityFilter* p_entityFiler, Entity* p_entity)
	{
		bool l_notify = true;

		_Core::VectorIteratorT<ComponentType> l_filteredComponentType = _Core::VectorT_buildIterator(&p_entityFiler->ListenedComponentTypes);
		while (_Core::VectorIteratorT_moveNext(&l_filteredComponentType))
		{
			if (!_Core::CompareT_contains(_Core::VectorT_buildIterator(&p_entity->Components), _Core::ComparatorT<Component*, ComponentType, void>{Component_comparator, l_filteredComponentType.Current}))
			{
				l_notify = false;
				break;
			};
		}

		if (l_notify)
		{
			_Core::CallbackT_call(&p_entityFiler->OnEntityThatMatchesComponentTypesRemoved, p_entity);
		}
	}

	void entityFilter_onComponentAttachedCallback(EntityFilter* p_entityFilter, Component* p_component)
	{
		entityFilter_notifyIfEntityMatches(p_entityFilter, p_component->AttachedEntity);
	};

	void entityFilterr_onComponentDetachedCallback(EntityFilter* p_entityFilter, Component* p_component)
	{
		entityFilter_notifyIfJustUnmatched(p_entityFilter, p_component->AttachedEntity);
	};

	void entityFilter_registerEvents(EntityFilter* p_entityFilter, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		_Core::CallbackT<EntityFilter, Component> l_onComponentAttachedEventListener = { entityFilter_onComponentAttachedCallback, p_entityFilter };
		_Core::CallbackT<EntityFilter, Component> l_onComponentDetachedEventListener = { entityFilterr_onComponentDetachedCallback, p_entityFilter };

		for (size_t i = 0; i < p_entityFilter->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::VectorT_at(&p_entityFilter->ListenedComponentTypes, i);

			if (!l_componentEvents->ComponentAttachedEvents.contains(*l_componentType))
			{
				_Core::ObserverT<Component> l_createdObserver;
				_Core::ObserverT_alloc(&l_createdObserver);
				l_componentEvents->ComponentAttachedEvents[*l_componentType] = l_createdObserver;
			}

			_Core::ObserverT_register(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) & l_onComponentAttachedEventListener);

			if (!l_componentEvents->ComponentDetachedEvents.contains(*l_componentType))
			{

				_Core::ObserverT<Component> l_createdObserver;
				_Core::ObserverT_alloc(&l_createdObserver);
				l_componentEvents->ComponentDetachedEvents[*l_componentType] = l_createdObserver;
			}
			_Core::ObserverT_register(&l_componentEvents->ComponentDetachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) & l_onComponentDetachedEventListener);
		}
	};

	void entityFilter_unregisterEvents(EntityFilter* p_entityFilter, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		_Core::CallbackT<EntityFilter, Component> l_onComponentAttachedEventListener = { entityFilter_onComponentAttachedCallback, p_entityFilter };
		_Core::CallbackT<EntityFilter, Component> l_onComponentDetachedEventListener = { entityFilterr_onComponentDetachedCallback, p_entityFilter };

		for (size_t i = 0; i < p_entityFilter->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::VectorT_at(&p_entityFilter->ListenedComponentTypes, i);
			_Core::ObserverT_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) & l_onComponentAttachedEventListener);
			_Core::ObserverT_unRegister(&l_componentEvents->ComponentDetachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) & l_onComponentDetachedEventListener);
		}

	}

}

#if comment 
void entityComponentListener_onComponentAttachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component);
void entityComponentListener_onComponentDetachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component);
void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity);
void entityComponentListener_pushEntityIfElligible(EntityConfigurableContainer* l_entityComponentListener, Component* l_comparedComponent);
void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity);

/**
	When initializing the @ref EntityConfigurableContainer, we check the presence of the listened @ref ComponentType for all @ref Entity and push the
	Entity to the Listener if successfull.
	This is to be sure that all already present Entities are checked if the system initiation is done after the Entity creation.
*/
void entityComponentListener_pushAllElligibleEntities(EntityConfigurableContainer* l_entityComponentListener, _Core::VectorT<Entity*>* p_queriedEntities);


void EntityConfigurableContainer_init(EntityConfigurableContainer* p_entityComponentListener, EntityConfigurableContainerInitInfo* p_entityComponentListenerInitInfo)
{
	p_entityComponentListener->ListenedComponentTypes = p_entityComponentListenerInitInfo->ListenedComponentTypes;
	_Core::VectorT_alloc(&p_entityComponentListener->FilteredEntities, 16);

	p_entityComponentListener->OnEntityThatMatchesComponentTypesAdded = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesAdded;
	p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesRemoved;

	entityComponentListener_pushAllElligibleEntities(p_entityComponentListener, &p_entityComponentListenerInitInfo->ECS->EntityContainer.Entities);
	entityComponentListener_registerEvents(p_entityComponentListener, p_entityComponentListenerInitInfo->ECS);
};

void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
{
	_Core::CallbackT<EntityConfigurableContainer, Component> l_onComponentAttachedEventListener = { entityComponentListener_onComponentAttachedCallback, p_entityComponentListener };
	_Core::CallbackT<EntityConfigurableContainer, Component> l_onComponentDetachedEventListener = { entityComponentListener_onComponentDetachedCallback, p_entityComponentListener };

	ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

	for (size_t i = 0; i < p_entityComponentListener->ListenedComponentTypes.Size; i++)
	{
		ComponentType* l_componentType = _Core::VectorT_at(&p_entityComponentListener->ListenedComponentTypes, i);
		_Core::ObserverT_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, _ECS::Component>*) & l_onComponentAttachedEventListener);
		_Core::ObserverT_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, _ECS::Component>*) & l_onComponentDetachedEventListener);
	}

	for (size_t i = 0; i < p_entityComponentListener->FilteredEntities.Size; i++)
	{
		Entity* l_entity = *_Core::VectorT_at(&p_entityComponentListener->FilteredEntities, i);
		entityComponentListener_removeEntity(p_entityComponentListener, l_entity);

	}

	_Core::VectorT_free(&p_entityComponentListener->ListenedComponentTypes);
	_Core::VectorT_free(&p_entityComponentListener->FilteredEntities);
};

void entityComponentListener_onComponentAttachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component)
{
	entityComponentListener_pushEntityIfElligible(p_entityComponentListener, p_component);
};

void entityComponentListener_onComponentDetachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component)
{

	if (_Core::CompareT_find(_Core::VectorT_buildIterator(&p_entityComponentListener->ListenedComponentTypes),
		_Core::ComparatorT<ComponentType, ComponentType, void>{ComponentType_comparator, p_component->ComponentType }).Current)
	{
		entityComponentListener_removeEntity(p_entityComponentListener, p_component->AttachedEntity);
	}
};

void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
{
	ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

	_Core::CallbackT<EntityConfigurableContainer, Component> l_onComponentAttachedEventListener = { entityComponentListener_onComponentAttachedCallback, p_entityComponentListener };
	_Core::CallbackT<EntityConfigurableContainer, Component> l_onComponentDetachedEventListener = { entityComponentListener_onComponentDetachedCallback, p_entityComponentListener };

	for (size_t i = 0; i < p_entityComponentListener->ListenedComponentTypes.Size; i++)
	{
		ComponentType* l_componentType = _Core::VectorT_at(&p_entityComponentListener->ListenedComponentTypes, i);

		if (!l_componentEvents->ComponentAttachedEvents.contains(*l_componentType))
		{
			_Core::ObserverT<Component> l_createdObserver;
			_Core::ObserverT_alloc(&l_createdObserver);
			l_componentEvents->ComponentAttachedEvents[*l_componentType] = l_createdObserver;
		}

		_Core::ObserverT_register(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) & l_onComponentAttachedEventListener);

		if (!l_componentEvents->ComponentDetachedEvents.contains(*l_componentType))
		{

			_Core::ObserverT<Component> l_createdObserver;
			_Core::ObserverT_alloc(&l_createdObserver);
			l_componentEvents->ComponentDetachedEvents[*l_componentType] = l_createdObserver;
		}
		_Core::ObserverT_register(&l_componentEvents->ComponentDetachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) & l_onComponentDetachedEventListener);
	}
};

void entityComponentListener_pushEntityIfElligible(_GameEngine::_ECS::EntityConfigurableContainer* l_entityComponentListener, _GameEngine::_ECS::Component* l_comparedComponent)
{
	if (_Core::CompareT_contains(_Core::VectorT_buildIterator(&l_entityComponentListener->ListenedComponentTypes),
		_Core::ComparatorT<ComponentType, ComponentType, void>{ComponentType_comparator, l_comparedComponent->ComponentType }))
	{
		bool l_addEntity = true;
		for (size_t i = 0; i < l_entityComponentListener->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::VectorT_at(&l_entityComponentListener->ListenedComponentTypes, i);
			bool l_filteredComponentTypeIsAnEntityComponent = false;

			for (size_t j = 0; j < l_comparedComponent->AttachedEntity->Components.Size; j++)
			{
				Component* l_component = *_Core::VectorT_at(&l_comparedComponent->AttachedEntity->Components, j);
				if (*l_component->ComponentType == *l_componentType)
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

void entityComponentListener_pushAllElligibleEntities(EntityConfigurableContainer* l_entityComponentListener, _Core::VectorT<Entity*>* p_queriedEntities)
{
	for (size_t i = 0; i < p_queriedEntities->Size; i++)
	{
		Entity** p_entity = _Core::VectorT_at(p_queriedEntities, i);
		for (size_t i = 0; i < l_entityComponentListener->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_askedComponentType = _Core::VectorT_at(&l_entityComponentListener->ListenedComponentTypes, i);
			bool l_componentTypeMatchFound = false;

			for (size_t j = 0; j < (*p_entity)->Components.Size; j++)
			{
				Component** l_component = _Core::VectorT_at(&(*p_entity)->Components, j);
				if (*l_askedComponentType == *(*l_component)->ComponentType)
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

		entityComponentListener_pushEntity(l_entityComponentListener, *p_entity);

	checkNextEntity:;
	}
};

void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity)
{
	if (!_Core::CompareT_contains(_Core::VectorT_buildIterator(&l_entityComponentListener->FilteredEntities),
		_Core::ComparatorT<Entity*, Entity*, void>{Entity_comparator, & p_entity}))
	{
		_Core::VectorT_pushBack(&l_entityComponentListener->FilteredEntities, &p_entity);
	}

	_Core::CallbackT_call(&l_entityComponentListener->OnEntityThatMatchesComponentTypesAdded, p_entity);
};

void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity)
{
	_Core::VectorIteratorT<Entity*> l_foundEntityIt = _Core::CompareT_find(_Core::VectorT_buildIterator(&p_entityComponentListener->FilteredEntities), _Core::ComparatorT<Entity*, Entity*, void>{Entity_comparator, & p_entity});
	if (l_foundEntityIt.Current)
	{
		_Core::VectorT_erase(&p_entityComponentListener->FilteredEntities, l_foundEntityIt.CurrentIndex);
		_Core::CallbackT_call(&p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved, p_entity);
	}
};
#endif