#include "Component.h"

#include <cstdlib>

namespace _GameEngine::_ECS
{

	void Component_free(Component* p_component)
	{
		if (p_component->Component_freeCallback)
		{
			p_component->Component_freeCallback(p_component);
		}
		delete p_component->Child;
	};

	Component Component_Build(ComponentType p_type, void* p_child)
	{
		Component l_component{};
		l_component.ComponentType = p_type;
		l_component.Child = p_child;
		return l_component;
	};
};