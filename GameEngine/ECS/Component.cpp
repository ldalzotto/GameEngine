#include "Component.h"

#include <cstdlib>

namespace _GameEngine::_ECS
{
	Component* Component_alloc(ComponentType& p_type, void* p_child)
	{
		Component* l_component = new Component();
		l_component->ComponentType = p_type;
		l_component->Child = p_child;
		return l_component;
	};

	void Component_free(Component** p_component)
	{
		if ((*p_component)->Component_freeCallback)
		{
			(*p_component)->Component_freeCallback((*p_component));
		}
		delete (*p_component)->Child;
		delete (*p_component);
	};

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, Component* p_component)
	{
		if (p_componentEvents->ComponentAttachedEvents.contains(p_component->ComponentType))
		{
			_Utils::Observer_broadcast(&p_componentEvents->ComponentAttachedEvents[p_component->ComponentType], p_component);
		}
	};

	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, Component* p_component)
	{
		if (p_componentEvents->ComponentDetachedEvents.contains(p_component->ComponentType))
		{
			_Utils::Observer_broadcast(&p_componentEvents->ComponentDetachedEvents[p_component->ComponentType], nullptr);
			/*
			std::vector<ComponentDetachedEvent>* l_componentDetachedEvents = &p_componentEvents->ComponentDetachedEvents[p_component->ComponentType];
			for (size_t i = 0; i < l_componentDetachedEvents->size(); i++)
			{
				l_componentDetachedEvents->at(i)(p_component);
			}
			*/
		}
	};
};