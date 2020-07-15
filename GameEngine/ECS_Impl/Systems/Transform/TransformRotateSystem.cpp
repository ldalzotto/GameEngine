#include "TransformRotateSystem.h"

#include "GameEngineApplicationInterface.h"
#include "Clock/Clock.hpp"

#include "DataStructures/Specifications/VectorT.hpp"

#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"

#include "Math/Quaternion/QuaternionMath.h"

namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy()
	{
		::_Core::VectorT<::_Core::SortedSequencerPriority> l_before;
		::_Core::VectorT_alloc(&l_before, 2);
		::_Core::VectorT_pushBack(&l_before, MeshDrawSystem_updatePriorityBefore());
		::_Core::VectorT_pushBack(&l_before, CameraSystem_getUpdatePriority());
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface);

	void TransformRotateSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;

		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		_Core::VectorT_alloc(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, 2);
		_Core::VectorT_pushBack(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, &TransformComponentType);
		_Core::VectorT_pushBack(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, &TransformRotateType);

		p_systemV2AllocInfo->Update.Priority = TransformRotateSystem_getUpdatePritoriy();
		p_systemV2AllocInfo->Update.OperationCallback.Function = TransformRotationSystemV2_update;
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface)
	{
		_ECS::SystemV2* l_transformRotateSystem = (_ECS::SystemV2*)p_transformRotateSystem;

		GameEngineApplicationInterface* l_gameEngineInterface = (GameEngineApplicationInterface*)p_gameEngineInterface;
		
		for (size_t i = 0; i < l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.Size; i++)
		{
			Entity** l_entity = _Core::VectorT_at(&l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities, i);
			TransformComponent* l_transform = ENTITY_GET_COMPONENT(TransformComponent, *l_entity);
			TransformRotate* l_transformRotate = ENTITY_GET_COMPONENT(TransformRotate, *l_entity);

			_Math::Quaternionf l_newLocalRotation;
			_Math::Quaternion_rotateAround(&l_transform->Transform.LocalRotation, &l_transformRotate->Axis, l_transformRotate->Speed * l_gameEngineInterface->Clock->DeltaTime, &l_newLocalRotation);

			_Math::Transform_setLocalRotation(&l_transform->Transform, l_newLocalRotation);
		}
	};

}