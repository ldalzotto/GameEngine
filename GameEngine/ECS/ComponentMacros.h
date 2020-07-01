#pragma once

namespace _GameEngine::_ECS
{
	struct Component;
}

namespace _GameEngine::_ECS
{
	template<class COMPONENT_TYPE>
	inline Component* Component_allocV2()
	{
		return Component_alloc(*_ECS::extractComponentType<COMPONENT_TYPE>(), sizeof(COMPONENT_TYPE));
	};

	template<class COMPONENT_TYPE>
	inline COMPONENT_TYPE* Component_getChild(Component* p_component)
	{
		return (COMPONENT_TYPE*) p_component->Child;
	};
}
