#pragma once

namespace _GameEngine::_ECS
{
	struct Component;
}

namespace _GameEngine::_ECS
{
	template<class COMPONENT_TYPE>
	inline COMPONENT_TYPE* Component_getChild(Component* p_component)
	{
		return (COMPONENT_TYPE*) p_component->Child;
	};

	template<class COMPONENT_TYPE>
	struct ComponentT
	{
		Component* Component;
		COMPONENT_TYPE* getChild() { return Component_getChild<COMPONENT_TYPE>(this->Component); };
	};

	template<class COMPONENT_TYPE>
	inline ComponentT<COMPONENT_TYPE> Component_allocV2()
	{
		return { Component_alloc(*_ECS::extractComponentType<COMPONENT_TYPE>(), sizeof(COMPONENT_TYPE)) };
	};
}
