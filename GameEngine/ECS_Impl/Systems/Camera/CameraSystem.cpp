#include "CameraSystem.h"

#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/VectorMath.h"
#include <vector>

#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "Render/RenderInterface.h"
#include "Render/LoopStep/CameraBufferSetupStep.h"

namespace _GameEngine::_ECS
{
	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface);

	_Utils::SortedSequencerPriority CameraSystem_getUpdatePriority()
	{
		_Core::VectorT<_Utils::SortedSequencerPriority>  l_before;
		l_before.alloc(1);
		{
			auto l_index = MeshDrawSystem_updatePriorityBefore();
			l_before.push_back(&l_index);
		}
		return _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void CameraSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->Update.Priority = CameraSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.Callback = cameraSystem_update;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&CameraType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformComponentType);
	};

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface)
	{
		_ECS::SystemV2* l_cameraSystem = (_ECS::SystemV2*)p_cameraSystem;
		if (l_cameraSystem->EntityConfigurableContainer.FilteredEntities.size() > 0)
		{
			Entity* l_entity = *l_cameraSystem->EntityConfigurableContainer.FilteredEntities.at(0);
			TransformComponent* p_transform = GET_COMPONENT(TransformComponent, l_entity);
			Camera* p_camera = GET_COMPONENT(Camera, l_entity);

			{
				_Math::Vector3f l_worldPosition = Transform_getWorldPosition(&p_transform->Transform);

				_Math::Vector3f l_target;
				_Math::Vector3f l_foward = Transform_getForward(&p_transform->Transform);
				_Math::Vector3f_add(&l_worldPosition, &l_foward, &l_target);

				_Math::Vector3f l_up = Transform_getUp(&p_transform->Transform);
				_Math::Vector3f_mul(&l_up, -1.0f, &l_up);

				_Math::Matrix4x4f l_lookAt;
				_Math::Matrixf4x4_lookAt(&l_worldPosition, &l_target, &l_up, &l_lookAt);
				_Math::Matrixf4x4_inv(&l_lookAt, &p_camera->ViewMatrix);
			}


			p_camera->RenderInterface->CameraBufferSetupStep->CameraProjection.Projection = p_camera->ProjectionMatrix;
			p_camera->RenderInterface->CameraBufferSetupStep->CameraProjection.View = p_camera->ViewMatrix;
			_Render::CameraBufferSetupStep_pushCameraPorjectionValueToGPU(p_camera->RenderInterface->CameraBufferSetupStep, p_camera->RenderInterface->Device);
		}
	};
}