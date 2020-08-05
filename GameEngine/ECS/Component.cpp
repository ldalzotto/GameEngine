#include "Component.h"

#include "Log/Log.hpp"
#include "ECS/ECS.h"
#include <cstdlib>
#include <string.h>

namespace _GameEngine::_ECS
{
	ComponentHeader* Component_alloc(ComponentType* p_type, size_t p_componentChildSize)
	{
		ComponentHeader* l_component = (ComponentHeader*)calloc(1, p_componentChildSize);
		l_component->ComponentType = p_type;
		return l_component;
	};

	void Component_free(ComponentHeader** p_component, ECS* p_ecs)
	{
		if ((*p_component)->OnComponentFree)
		{
			(*p_component)->OnComponentFree(*p_component, p_ecs);
		}
		(*p_component)->AttachedEntity = nullptr;
		free (*p_component);
		*p_component = nullptr;
	};

	bool Component_comparator(ComponentHeader** left, ComponentType* right, void*)
	{
		return *(*left)->ComponentType == *right;
	};

	void ComponentEvents_free(ComponentEvents* p_componentEvents, ECS* p_ecs)
	{
#ifndef NDEBUG
		if(p_componentEvents->ComponentAttachedEvents.size() > 0 || p_componentEvents->ComponentDetachedEvents.size() > 0)
		{ 
			MYLOG_PUSH(p_ecs->MyLog, ::_Core::LogLevel::LOG_WARN, "Potential memory leak. When disposing ComponentEvents, there was still observers listening to component events. Please, consider unregister before destroying.");
		}
#endif
		for (auto&& p_componentAttachedEventsEntry : p_componentEvents->ComponentAttachedEvents)
		{
			_Core::ObserverT_free(&p_componentAttachedEventsEntry.second);
		}
		for (auto&& p_cimponentDetachedEventsEntry : p_componentEvents->ComponentDetachedEvents)
		{
			_Core::ObserverT_free(&p_cimponentDetachedEventsEntry.second);
		}
	};

	void ComponentEvents_onComponentAttached(ComponentEvents* p_componentEvents, ComponentHeader* p_component)
	{
		if (p_componentEvents->ComponentAttachedEvents.contains(*p_component->ComponentType))
		{
			_Core::ObserverT_broadcast(&p_componentEvents->ComponentAttachedEvents[*p_component->ComponentType], p_component);
		}
	};

	void ComponentEvents_onComponentDetached(ComponentEvents* p_componentEvents, ComponentHeader* p_component)
	{
		if (p_componentEvents->ComponentDetachedEvents.contains(*p_component->ComponentType))
		{
			_Core::ObserverT_broadcast(&p_componentEvents->ComponentDetachedEvents[*p_component->ComponentType], p_component);
		}
	};
};