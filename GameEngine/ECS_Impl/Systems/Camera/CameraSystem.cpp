#include "CameraSystem.h"

#include "Log/Log.h"
#include "Log/LogFormatters.h"

#include "glm/gtc/matrix_transform.hpp"
#include <vector>

#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "EngineSequencers/EngineSequencers.h"

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

	void CameraSystem_free(System* p_cameraSystem)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem->_child;
		EntityConfigurableContainer_free(&l_cameraSystem->EntityConfigurableContainer, l_cameraSystem->ECS);
		delete l_cameraSystem;
		p_cameraSystem->_child = nullptr;
	}

	System* CameraSystem_alloc(ECS* p_ecs, _Render::RenderInterface* p_renderInterface)
	{
		SystemAllocInfo l_systemAllocInfo{};
		l_systemAllocInfo.Child = new CameraSystem();
		l_systemAllocInfo.OnSystemFree = CameraSystem_free;
		System* l_system = SystemContainer_allocSystem(&p_ecs->SystemContainer, &l_systemAllocInfo);

		CameraSystem* l_cameraSystem = (CameraSystem*)l_system->_child;

		l_cameraSystem->ECS = p_ecs;
		l_cameraSystem->RenderInterface = p_renderInterface;
		l_cameraSystem->Update.Priority = CameraSystem_getUpdatePriority();
		l_cameraSystem->Update.Callback = cameraSystem_update;
		l_cameraSystem->Update.Closure = l_cameraSystem;

		_Utils::SortedSequencer_addOperation(&p_ecs->UpdateSequencer->UpdateSequencer, &l_cameraSystem->Update);
		
		EntityConfigurableContainerInitInfo l_entityConfigurableContainerInfo{};
		l_entityConfigurableContainerInfo.ECS = p_ecs;
		l_entityConfigurableContainerInfo.ListenedComponentTypes.alloc(2);
		l_entityConfigurableContainerInfo.ListenedComponentTypes.push_back(&CameraType);
		l_entityConfigurableContainerInfo.ListenedComponentTypes.push_back(&TransformType);
		EntityConfigurableContainer_init(&l_cameraSystem->EntityConfigurableContainer, &l_entityConfigurableContainerInfo);

		return l_system;
	};

	void cameraSystem_update(void* p_cameraSystem, void* p_delta)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		if (l_cameraSystem->EntityConfigurableContainer.FilteredEntities.size() > 0)
		{
			Entity* l_entity = *l_cameraSystem->EntityConfigurableContainer.FilteredEntities.at(0);
			Transform* p_transform = GET_COMPONENT(Transform, l_entity);
			Camera* p_camera = GET_COMPONENT(Camera, l_entity);

			p_camera->ViewMatrix = glm::lookAt(Transform_getWorldPosition(p_transform), glm::vec3(0.0f) /* Transform_getWorldPosition(p_transform) + Transform_getForward(p_transform)*/, Transform_getUp(p_transform));

			l_cameraSystem->RenderInterface->CameraBufferSetupStep->CameraProjection.Projection = p_camera->ProjectionMatrix;
			l_cameraSystem->RenderInterface->CameraBufferSetupStep->CameraProjection.View = p_camera->ViewMatrix;
			_Render::CameraBufferSetupStep_pushCameraPorjectionValueToGPU(l_cameraSystem->RenderInterface->CameraBufferSetupStep, l_cameraSystem->RenderInterface->Device);
		}
	};
}