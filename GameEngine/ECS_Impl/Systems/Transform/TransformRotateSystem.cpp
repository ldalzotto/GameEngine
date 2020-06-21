#include "TransformRotateSystem.h"

#include "GameEngineApplicationInterface.h"
#include "Clock/Clock.h"
#include "MyLog/MyLog.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"

#include "Math/Quaternion/QuaternionMath.h"

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy()
	{
		_Core::VectorT<_Utils::SortedSequencerPriority>  l_before;
		l_before.alloc(2);
		{
			auto l_index = MeshDrawSystem_updatePriorityBefore();
			l_before.push_back(&l_index);
		}
		{
			auto l_index = CameraSystem_getUpdatePriority();
			l_before.push_back(&l_index);
		}
		return _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface);

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

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface)
	{
		_ECS::SystemV2* l_transformRotateSystem = (_ECS::SystemV2*)p_transformRotateSystem;

		GameEngineApplicationInterface* l_gameEngineInterface = (GameEngineApplicationInterface*)p_gameEngineInterface;

		// _Log::MyLog_pushLog(l_gameEngineInterface->Log, _Log::LogLevel::DEBUG, "This is a debug");
		
		for (size_t i = 0; i < l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			Entity** l_entity = l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.at(i);
			Transform* l_transform = GET_COMPONENT(Transform, *l_entity);
			TransformRotate* l_transformRotate = GET_COMPONENT(TransformRotate, *l_entity);

			_Math::Quaternionf l_newLocalRotation;
			_Math::Quaternion_rotateAround(&l_transform->LocalRotation, &l_transformRotate->Axis, l_transformRotate->Speed * l_gameEngineInterface->Clock->DeltaTime, &l_newLocalRotation);

			Transform_setLocalRotation(l_transform, l_newLocalRotation);
		}
	};

}