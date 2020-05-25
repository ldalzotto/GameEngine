#pragma once

#include "Component.h"
#include "Entity.h"
#include "Utils/Observer/Observer.h"

namespace _GameEngine::_ECS
{

	struct EntityComponentListener
	{
		std::vector<ComponentType> FilteredTypes;
		_Utils::Subject ComponentAttachedEventListener;
		_Utils::Subject ComponentDetachedEventListener;
		std::vector<Entity*> FilteredEntities;
		void(*OnEntityAddedCallback)(Entity*);
	};

	struct EntityComponentListenerInitInfo
	{
		std::vector<ComponentType> FilteredTypes;
		ComponentEvents* ComponentEvents;
		void(*OnEntityAddedCallback)(Entity*);
	};

	void EntityComponentListener_init(EntityComponentListener* p_entityComponentListener, EntityComponentListenerInitInfo* p_entityComponentListenerInitInfo);
	void EntityComponentListener_free(EntityComponentListener* p_entityComponentListener, ComponentEvents* p_componentEvents);
}