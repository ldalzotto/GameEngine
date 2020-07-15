#include "Component.h"

#include "Log/Log.hpp"
#include "ECS/ECS.h"
#include <cstdlib>

namespace _GameEngine::_ECS
{
	Component* Component_alloc(ComponentType& p_type, size_t p_componentChildSize)
	{
		Component* l_component = new Component();
		l_component->ComponentType = p_type;
		l_component->Child = calloc(1, p_componentChildSize);
		Core_ObserverAlloc(&l_component->OnComponentFree);
		return l_component;
	};

	void Component_free(Component** p_component)
	{
		Core_Observer_broadcast(&(*p_component)->OnComponentFree, NULL);
		free((*p_component)->Child);
		(*p_component)->Child = nullptr;
		(*p_component)->AttachedEntity = nullptr;
		Core_ObserverFree(&(*p_component)->OnComponentFree);
		delete (*p_component);
		*p_component = nullptr;
	};

	bool Component_comparator(Component** left, ComponentType* right, void*)
	{
		return (*left)->ComponentType == *right;
	};

	void ComponentEvents_free(ComponentEvents* p_componentEvents, ECS* p_ecs)
	{
#ifndef NDEBUG
		if(p_componentEvents->ComponentAttachedEvents.size() > 0 || p_componentEvents->ComponentDetachedEvents.size() > 0)
		{ 
			MYLOG_PUSH(p_ecs->MyLog, ::_Core::LogLevel::WARN, "Potential memory leak. When disposing ComponentEvents, there was still observers listening to component events. Please, consider unregister before destroying.");
		}
#endif
		for (auto&& p_componentAttachedEventsEntry : p_componentEvents->ComponentAttachedEvents)
		{
			Core_ObserverFree(&p_componentAttachedEventsEntry.second);
		}
		for (auto&& p_cimponentDetachedEventsEntry : p_componentEvents->ComponentDetachedEvents)
		{
			Core_ObserverFree(&p_cimponentDetachedEventsEntry.second);
		}
	};

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, Component* p_component)
	{
		if (p_componentEvents->ComponentAttachedEvents.contains(p_component->ComponentType))
		{
			Core_Observer_broadcast(&p_componentEvents->ComponentAttachedEvents[p_component->ComponentType], p_component);
		}
	};

	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, Component* p_component)
	{
		if (p_componentEvents->ComponentDetachedEvents.contains(p_component->ComponentType))
		{
			Core_Observer_broadcast(&p_componentEvents->ComponentDetachedEvents[p_component->ComponentType], p_component);
		}
	};
};