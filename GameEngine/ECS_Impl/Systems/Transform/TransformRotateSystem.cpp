#include "TransformRotateSystem.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS/ECS.h"

namespace _GameEngine::_ECS
{
	void TransformRotateSystem_init(TransformRotateSystem* p_transformRotateSystem, ECS* p_ecs)
	{
		p_transformRotateSystem->ECS = p_ecs;

		EntityConfigurableContainerInitInfo l_entityConfigurableContainerInitInfo{};
		l_entityConfigurableContainerInitInfo.ListenedComponentTypes = { TransformType, TransformRotateType };
		l_entityConfigurableContainerInitInfo.ECS = p_ecs;
		EntityConfigurableContainer_init(&p_transformRotateSystem->EntityConfigurableContainer, &l_entityConfigurableContainerInitInfo);
	};

	void TransformRotateSystem_free(System* p_system)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_system->_child;
		EntityConfigurableContainer_free(&l_transformRotateSystem->EntityConfigurableContainer, l_transformRotateSystem->ECS);
		delete l_transformRotateSystem;
		p_system->_child = nullptr;
	};


	void TransformRotationSystem_update(void* p_transformRotateSystem, float p_delta)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		for (Entity* l_entity : l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities)
		{
			Transform* l_transform = GET_COMPONENT(Transform, l_entity);
			TransformRotate* l_transformRotate = GET_COMPONENT(TransformRotate, l_entity);
			Transform_setLocalRotation(l_transform, l_transform->LocalRotation * glm::quat(l_transformRotate->Axis * l_transformRotate->Speed * p_delta));
		}
	};

	System* TransformRotateSystem_alloc(ECS* p_ecs)
	{
		SystemAllocInfo l_systemAllocInfo{};
		l_systemAllocInfo.Child = new TransformRotateSystem();
		l_systemAllocInfo.UpdateFunction = TransformRotationSystem_update;
		l_systemAllocInfo.OnSystemFree = TransformRotateSystem_free;
		System* l_system = SystemContainer_allocSystem(&p_ecs->SystemContainer, &l_systemAllocInfo);
		TransformRotateSystem* l_transformRotationSystem = (TransformRotateSystem*)l_system->_child;
		TransformRotateSystem_init(l_transformRotationSystem, p_ecs);
		return l_system;
	};
}