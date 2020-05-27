#include "TransformRotateSystem.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"

namespace _GameEngine::_ECS
{
	void TransformRotateSystem_init(TransformRotateSystem* p_transformRotateSystem, ECS* p_ecs)
	{
		EntityConfigurableContainerInitInfo l_entityConfigurableContainerInitInfo{};
		l_entityConfigurableContainerInitInfo.ListenedComponentTypes = { TransformType, TransformRotateType };
		l_entityConfigurableContainerInitInfo.ECS = p_ecs;
		EntityConfigurableContainer_init(&p_transformRotateSystem->EntityConfigurableContainer, &l_entityConfigurableContainerInitInfo);
	};

	void TransformRotateSystem_free(TransformRotateSystem* p_transformRotateSystem)
	{
		EntityConfigurableContainer_free(&p_transformRotateSystem->EntityConfigurableContainer, p_transformRotateSystem->ECS);
	};

	void TransformRotationSystem_update(TransformRotateSystem* p_transformRotateSystem, float p_delta)
	{
		for (Entity* l_entity : p_transformRotateSystem->EntityConfigurableContainer.FilteredEntities)
		{
			Transform* l_transform = (Transform*)Entity_getComponent(l_entity, TransformType)->Child;
			TransformRotate* l_transformRotate = (TransformRotate*)Entity_getComponent(l_entity, TransformRotateType)->Child;
			Transform_setLocalRotation(l_transform, l_transform->LocalRotation * glm::quat(l_transformRotate->Axis * l_transformRotate->Speed * p_delta));
		}
	};
}