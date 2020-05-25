#include "ECS/System.h"

#include "Utils/Algorithm/Algorithm.h"
#include <vector>
#include "Entity.h"

namespace _GameEngine::_ECS
{

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityComponentListener* l_entityComponentListener = (EntityComponentListener*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;

		
		size_t l_foundIndex = _Utils::Vector_containsElementEquals(l_entityComponentListener->FilteredTypes, l_component->ComponentType);
		if (l_foundIndex != -1)
		{
			bool l_addEntity = true;
			for (ComponentType& l_componentType : l_entityComponentListener->FilteredTypes)
			{
				bool l_filteredComponentTypeIsAnEntityComponent = false;
				for (auto&& l_entityComponentEntry : l_component->AttachedEntity->Components)
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
				if (_Utils::Vector_containsElementEquals(l_entityComponentListener->FilteredEntities, l_component->AttachedEntity) == -1)
				{
					l_entityComponentListener->FilteredEntities.emplace_back(l_component->AttachedEntity);
				}

				if (l_entityComponentListener->OnEntityAddedCallback)
				{
					l_entityComponentListener->OnEntityAddedCallback(l_component->AttachedEntity);
				}
			}
		}
	};

	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityComponentListener* l_entityComponentListener = (EntityComponentListener*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;

		size_t l_foundIndex = _Utils::Vector_containsElementEquals(l_entityComponentListener->FilteredTypes, l_component->ComponentType);
		if (l_foundIndex != -1)
		{
			_Utils::Vector_eraseElementEquals(l_entityComponentListener->FilteredEntities, l_component->AttachedEntity);
		}
	};

	void EntityComponentListener_init(EntityComponentListener* p_entityComponentListener, EntityComponentListenerInitInfo* p_entityComponentListenerInitInfo)
	{
		p_entityComponentListener->FilteredTypes = p_entityComponentListenerInitInfo->FilteredTypes;
		p_entityComponentListener->OnEntityAddedCallback = p_entityComponentListenerInitInfo->OnEntityAddedCallback;
		p_entityComponentListener->ComponentAttachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->ComponentAttachedEventListener.Callback = entityComponentListener_onComponentAttachedCallback;

		p_entityComponentListener->ComponentDetachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->ComponentDetachedEventListener.Callback = entityComponentListener_onComponentAttachedCallback;

		for (ComponentType& l_componentType : p_entityComponentListener->FilteredTypes)
		{
			if (!p_entityComponentListenerInitInfo->ComponentEvents->ComponentAttachedEvents.contains(l_componentType))
			{
				p_entityComponentListenerInitInfo->ComponentEvents->ComponentAttachedEvents[l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&p_entityComponentListenerInitInfo->ComponentEvents->ComponentAttachedEvents[l_componentType], &p_entityComponentListener->ComponentAttachedEventListener);


			if (!p_entityComponentListenerInitInfo->ComponentEvents->ComponentDetachedEvents.contains(l_componentType))
			{
				p_entityComponentListenerInitInfo->ComponentEvents->ComponentDetachedEvents[l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&p_entityComponentListenerInitInfo->ComponentEvents->ComponentDetachedEvents[l_componentType], &p_entityComponentListener->ComponentDetachedEventListener);
		}
	};

	void EntityComponentListener_free(EntityComponentListener* p_entityComponentListener, ComponentEvents* p_componentEvents)
	{
		for (ComponentType& l_componentType : p_entityComponentListener->FilteredTypes)
		{
			_Utils::Observer_unRegister(&p_componentEvents->ComponentAttachedEvents[l_componentType], &p_entityComponentListener->ComponentAttachedEventListener);
			_Utils::Observer_unRegister(&p_componentEvents->ComponentDetachedEvents[l_componentType], &p_entityComponentListener->ComponentDetachedEventListener);
		}
		p_entityComponentListener->FilteredEntities.clear();
	};

}