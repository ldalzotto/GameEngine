#include "TransformRotateSystem.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"

#include "Math/Quaternion/QuaternionTransform.h"

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy()
	{
		std::vector<_Utils::SortedSequencerPriority> l_before{
			MeshDrawSystem_getUpdatePriority(),
			CameraSystem_getUpdatePriority()
		};
		return _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_delta);

	void TransformRotateSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;

		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformRotateType);

		p_systemV2AllocInfo->Update.Priority = TransformRotateSystem_getUpdatePritoriy();
		p_systemV2AllocInfo->Update.Callback = TransformRotationSystemV2_update;
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_delta)
	{
		_ECS::SystemV2* l_transformRotateSystem = (_ECS::SystemV2*)p_transformRotateSystem;
		float l_delta = *(float*)p_delta;

		for (size_t i = 0; i < l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			Entity** l_entity = l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.at(i);
			Transform* l_transform = GET_COMPONENT(Transform, *l_entity);
			TransformRotate* l_transformRotate = GET_COMPONENT(TransformRotate, *l_entity);

			_Math::Quaternionf l_newLocalRotation;
			_Math::Quaternion_rotateAround(&l_transform->LocalRotation, &l_transformRotate->Axis, l_transformRotate->Speed * l_delta, &l_newLocalRotation);

			Transform_setLocalRotation(l_transform, l_newLocalRotation);
		}
	};

}