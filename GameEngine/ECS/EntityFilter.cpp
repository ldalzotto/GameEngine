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


	inline void EntityFilter_initCallbacks(EntityFilter* p_entityFiler, ECS* p_ecs, void* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(void*, Entity*), void(*p_OnEntityThatMatchesComponentTypesRemoved)(void*, Entity*))
	{
		p_entityFiler->OnEntityThatMatchesComponentTypesAdded = { p_onEntityThatMatchesComponentTypesAdded, p_closure };
		p_entityFiler->OnEntityThatMatchesComponentTypesRemoved = { p_OnEntityThatMatchesComponentTypesRemoved, p_closure };
		entityFilter_notifyAllMatchedEntities(p_entityFiler, &p_ecs->EntityContainer.Entities);
		entityFilter_registerEvents(p_entityFiler, p_ecs);
	};

	void EntityFilter_alloc_1c(EntityFilter* p_entityFiler, ECS* p_ecs,
		ComponentType* l_listenedType1,
		void* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(void*, Entity*), 
		void(*p_OnEntityThatMatchesComponentTypesRemoved)(void*, Entity*))
	{
		_Core::ArrayT_alloc(&p_entityFiler->ListenedComponentTypes, 1);
		_Core::ArrayT_pushBack(&p_entityFiler->ListenedComponentTypes, l_listenedType1);
		EntityFilter_initCallbacks(p_entityFiler, p_ecs, p_closure, p_onEntityThatMatchesComponentTypesAdded, p_OnEntityThatMatchesComponentTypesRemoved);
	};
	
	void EntityFilter_alloc_2c(EntityFilter* p_entityFiler, ECS* p_ecs,
		ComponentType* l_listenedType1, ComponentType* l_listenedType2,
		void* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(void*, Entity*), 
		void(*p_OnEntityThatMatchesComponentTypesRemoved)(void*, Entity*))
	{
		_Core::ArrayT_alloc(&p_entityFiler->ListenedComponentTypes, 2);
		_Core::ArrayT_pushBack(&p_entityFiler->ListenedComponentTypes, l_listenedType1);
		_Core::ArrayT_pushBack(&p_entityFiler->ListenedComponentTypes, l_listenedType2);
		EntityFilter_initCallbacks(p_entityFiler, p_ecs, p_closure, p_onEntityThatMatchesComponentTypesAdded, p_OnEntityThatMatchesComponentTypesRemoved);
	}
	
	void EntityFilter_free(EntityFilter* p_entityFiler, ECS* p_ecs)
	{
		entityFilter_unregisterEvents(p_entityFiler, p_ecs);
		_Core::ArrayT_free(&p_entityFiler->ListenedComponentTypes);
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

		_Core::VectorIteratorT<ComponentType> l_filteredComponentType = _Core::ArrayT_buildIterator(&p_entityFiler->ListenedComponentTypes);
		while (_Core::VectorIteratorT_moveNext(&l_filteredComponentType))
		{
			if (!_Core::CompareT_contains(_Core::VectorT_buildIterator(&p_entity->Components), _Core::ComparatorT<ComponentHeader*, ComponentType, void>{Component_comparator, l_filteredComponentType.Current}))
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

		_Core::VectorIteratorT<ComponentType> l_filteredComponentType = _Core::ArrayT_buildIterator(&p_entityFiler->ListenedComponentTypes);
		while (_Core::VectorIteratorT_moveNext(&l_filteredComponentType))
		{
			if (!_Core::CompareT_contains(_Core::VectorT_buildIterator(&p_entity->Components), _Core::ComparatorT<ComponentHeader*, ComponentType, void>{Component_comparator, l_filteredComponentType.Current}))
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

	void entityFilter_onComponentAttachedCallback(EntityFilter* p_entityFilter, ComponentHeader* p_component)
	{
		entityFilter_notifyIfEntityMatches(p_entityFilter, p_component->AttachedEntity);
	};

	void entityFilterr_onComponentDetachedCallback(EntityFilter* p_entityFilter, ComponentHeader* p_component)
	{
		entityFilter_notifyIfJustUnmatched(p_entityFilter, p_component->AttachedEntity);
	};

	void entityFilter_registerEvents(EntityFilter* p_entityFilter, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		_Core::CallbackT<EntityFilter, ComponentHeader> l_onComponentAttachedEventListener = { entityFilter_onComponentAttachedCallback, p_entityFilter };
		_Core::CallbackT<EntityFilter, ComponentHeader> l_onComponentDetachedEventListener = { entityFilterr_onComponentDetachedCallback, p_entityFilter };

		for (size_t i = 0; i < p_entityFilter->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::ArrayT_at(&p_entityFilter->ListenedComponentTypes, i);

			if (!l_componentEvents->ComponentAttachedEvents.contains(*l_componentType))
			{
				_Core::ObserverT<ComponentHeader> l_createdObserver;
				_Core::ObserverT_alloc(&l_createdObserver);
				l_componentEvents->ComponentAttachedEvents[*l_componentType] = l_createdObserver;
			}

			_Core::ObserverT_register(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, ComponentHeader>*) & l_onComponentAttachedEventListener);

			if (!l_componentEvents->ComponentDetachedEvents.contains(*l_componentType))
			{

				_Core::ObserverT<ComponentHeader> l_createdObserver;
				_Core::ObserverT_alloc(&l_createdObserver);
				l_componentEvents->ComponentDetachedEvents[*l_componentType] = l_createdObserver;
			}
			_Core::ObserverT_register(&l_componentEvents->ComponentDetachedEvents[*l_componentType], (_Core::CallbackT<void, ComponentHeader>*) & l_onComponentDetachedEventListener);
		}
	};

	void entityFilter_unregisterEvents(EntityFilter* p_entityFilter, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		_Core::CallbackT<EntityFilter, ComponentHeader> l_onComponentAttachedEventListener = { entityFilter_onComponentAttachedCallback, p_entityFilter };
		_Core::CallbackT<EntityFilter, ComponentHeader> l_onComponentDetachedEventListener = { entityFilterr_onComponentDetachedCallback, p_entityFilter };

		for (size_t i = 0; i < p_entityFilter->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::ArrayT_at(&p_entityFilter->ListenedComponentTypes, i);
			_Core::ObserverT_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, ComponentHeader>*) & l_onComponentAttachedEventListener);
			_Core::ObserverT_unRegister(&l_componentEvents->ComponentDetachedEvents[*l_componentType], (_Core::CallbackT<void, ComponentHeader>*) & l_onComponentDetachedEventListener);
		}

	}

}

