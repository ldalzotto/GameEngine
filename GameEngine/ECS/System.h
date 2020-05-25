#pragma once

#include "Component.h"
#include "Entity.h"
#include "Utils/Observer/Observer.h"

namespace _GameEngine::_ECS
{

	struct EntityComponentListener
	{
		std::vector<ComponentType> ListenedComponentTypes;
		std::vector<Entity*> FilteredEntities;
		void(*OnEntityThatMatchesComponentTypesAdded)(Entity*);
		void(*OnEntityThatMatchesComponentTypesRemoved)(Entity*);

		/**
			Called when a @ref Component with type contained in @ref ListenedComponentTypes is attached.
		*/
		_Utils::Subject ComponentAttachedEventListener;

		/**
			Called when a @ref Component with type contained in @ref ListenedComponentTypes is detached.
		*/
		_Utils::Subject ComponentDetachedEventListener;
	};

	struct EntityComponentListenerInitInfo
	{
		std::vector<ComponentType> ListenedComponentTypes;
		void(*OnEntityThatMatchesComponentTypesAdded)(Entity*);
		void(*OnEntityThatMatchesComponentTypesRemoved)(Entity*);

		ComponentEvents* ComponentEvents;
		EntityContainer* EntityContainer;
	};

	void EntityComponentListener_init(EntityComponentListener* p_entityComponentListener, EntityComponentListenerInitInfo* p_entityComponentListenerInitInfo);
	void EntityComponentListener_free(EntityComponentListener* p_entityComponentListener, ComponentEvents* p_componentEvents);
}