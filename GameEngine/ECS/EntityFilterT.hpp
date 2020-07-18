#pragma once

#include "EntityFilter.hpp"
#include "ComponentT.hpp"


#define ENTITy_FILTER_T_DECLARE_STRUCT(EntityFilterTypeName) \
struct EntityFilterTypeName \
{ \
	_Core::ArrayT<ComponentType> ListenedComponentTypes; \
	_Core::CallbackT<CLOSURE_TYPE, Entity> OnEntityThatMatchesComponentTypesAdded; \
	_Core::CallbackT<CLOSURE_TYPE, Entity> OnEntityThatMatchesComponentTypesRemoved; \
};

namespace _GameEngine::_ECS
{
	template <typename CLOSURE_TYPE, typename COMPONENT_TYPE_1>
	ENTITy_FILTER_T_DECLARE_STRUCT(EntityFilterT_1C)

	template <typename CLOSURE_TYPE, typename COMPONENT_TYPE_1>
	inline void EntityFilterT_alloc(EntityFilterT_1C<CLOSURE_TYPE, COMPONENT_TYPE_1>* p_entityFiler, ECS* p_ecs,
		CLOSURE_TYPE* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(CLOSURE_TYPE*, Entity*), void(*p_OnEntityThatMatchesComponentTypesRemoved)(CLOSURE_TYPE*, Entity*))
	{
		EntityFilter_alloc_1c((EntityFilter*)p_entityFiler, p_ecs, ComponentT_getComponentType<COMPONENT_TYPE_1>(), (void*)p_closure,
			(void(*)(void*, Entity*)) p_onEntityThatMatchesComponentTypesAdded,
			(void(*)(void*, Entity*)) p_OnEntityThatMatchesComponentTypesRemoved);
	};
	template <typename CLOSURE_TYPE, typename COMPONENT_TYPE_1>
	inline void EntityFilterT_free(EntityFilterT_1C<CLOSURE_TYPE, COMPONENT_TYPE_1>* p_entityFiler, ECS* p_ecs)
	{
		EntityFilter_free((EntityFilter*)p_entityFiler, p_ecs);
	};
	// inline void EntityFilterT_

	template <typename CLOSURE_TYPE, typename COMPONENT_TYPE_1, typename COMPONENT_TYPE_2>
	ENTITy_FILTER_T_DECLARE_STRUCT(EntityFilterT_2C)

		template <typename CLOSURE_TYPE, typename COMPONENT_TYPE_1, typename COMPONENT_TYPE_2>
	inline void EntityFilterT_alloc(EntityFilterT_2C<CLOSURE_TYPE, COMPONENT_TYPE_1, COMPONENT_TYPE_2>* p_entityFiler, ECS* p_ecs,
		CLOSURE_TYPE* p_closure, void(*p_onEntityThatMatchesComponentTypesAdded)(CLOSURE_TYPE*, Entity*), void(*p_OnEntityThatMatchesComponentTypesRemoved)(CLOSURE_TYPE*, Entity*))
	{
		EntityFilter_alloc_2c((EntityFilter*)p_entityFiler, p_ecs, ComponentT_getComponentType<COMPONENT_TYPE_1>(), ComponentT_getComponentType<COMPONENT_TYPE_2>(), (void*)p_closure,
			(void(*)(void*, Entity*)) p_onEntityThatMatchesComponentTypesAdded,
			(void(*)(void*, Entity*)) p_OnEntityThatMatchesComponentTypesRemoved);
	};

	template <typename CLOSURE_TYPE, typename COMPONENT_TYPE_1, typename COMPONENT_TYPE_2>
	inline void EntityFilterT_free(EntityFilterT_2C<CLOSURE_TYPE, COMPONENT_TYPE_1, COMPONENT_TYPE_2>* p_entityFiler, ECS* p_ecs)
	{
		EntityFilter_free((EntityFilter*)p_entityFiler, p_ecs);
	};
}
