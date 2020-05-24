#include "Component.h"

#include <cstdlib>

namespace _GameEngine::_ECS
{
	void Component_init(Component* p_component, ComponentType p_cType, size_t p_childSize)
	{
		p_component->ComponentType = p_cType;
		p_component->Child = malloc(p_childSize);
	};

	void Component_free(Component* p_component)
	{
		if (p_component->Component_freeCallback)
		{
			p_component->Component_freeCallback(p_component);
		}
		free(p_component->Child);
	};
};