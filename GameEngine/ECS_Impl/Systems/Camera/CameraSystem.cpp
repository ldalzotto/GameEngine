#include "CameraSystem.h"

#include "Log/Log.h"
#include "Log/LogFormatters.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Math/Matrix/MatrixTransform.h"
#include <vector>

#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "Render/RenderInterface.h"
#include "Render/LoopStep/CameraBufferSetupStep.h"

namespace _GameEngine::_ECS
{
	void cameraSystem_update(void* p_cameraSystem, void* p_delta);

	_Utils::SortedSequencerPriority CameraSystem_getUpdatePriority()
	{
		std::vector<_Utils::SortedSequencerPriority> l_before
		{
			MeshDrawSystem_getUpdatePriority()
		};
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
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformType);
	};

	void cameraSystem_update(void* p_cameraSystem, void* p_delta)
	{
		_ECS::SystemV2* l_cameraSystem = (_ECS::SystemV2*)p_cameraSystem;
		if (l_cameraSystem->EntityConfigurableContainer.FilteredEntities.size() > 0)
		{
			Entity* l_entity = *l_cameraSystem->EntityConfigurableContainer.FilteredEntities.at(0);
			Transform* p_transform = GET_COMPONENT(Transform, l_entity);
			Camera* p_camera = GET_COMPONENT(Camera, l_entity);

			_Math::Vector3f l_worldPosition = Transform_getWorldPosition(p_transform);
			glm::vec3 l_worldPositionGLM;
			_Math::Vector3f_toGLM(&l_worldPosition, &l_worldPositionGLM);

			_Math::Vector3f l_up = Transform_getUp(p_transform);
			glm::vec3 l_upGLM;
			{
				_Math::Vector3f_toGLM(&l_up, &l_upGLM);
			}


			_Math::Vector3f l_target = { 0.0f, 0.0f, 0.0f };
			p_camera->ViewMatrix = glm::lookAt(l_worldPositionGLM, glm::vec3(0.0f) /* Transform_getWorldPosition(p_transform) + Transform_getForward(p_transform)*/, l_upGLM);

			{
				_Math::Vector3f l_target = { 0.0f, 0.0f, 0.0f };
				_Math::Matrix4x4f l_view;
				_Math::Matrixf4x4_lookAtViewMatrix(&l_worldPosition, &l_target, &l_up, &l_view);
				_Math::Matrix4x4f l_viewInv;
				_Math::Matrixf4x4_inv(&l_view, &l_viewInv);
				p_camera->ViewMatrix = l_viewInv.toGLM();
			}
			

			p_camera->RenderInterface->CameraBufferSetupStep->CameraProjection.Projection = p_camera->ProjectionMatrix;
			p_camera->RenderInterface->CameraBufferSetupStep->CameraProjection.View = p_camera->ViewMatrix;
			_Render::CameraBufferSetupStep_pushCameraPorjectionValueToGPU(p_camera->RenderInterface->CameraBufferSetupStep, p_camera->RenderInterface->Device);
		}
	};
}