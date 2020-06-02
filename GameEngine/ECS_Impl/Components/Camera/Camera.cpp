#include "Camera.h"

#include "Render/SwapChain/SwapChain.h"
#include "Render/LoopStep/CameraBufferSetupStep.h"

#include "ECS/Component.h"

#include "glm/gtc/matrix_transform.hpp"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "Camera";

	void camera_onSwapChainBuild(void* p_camera, void* p_swapChain)
	{
		Camera* l_camera = (Camera*)p_camera;
		Camera_buildProjectionMatrix(l_camera);
		_Render::CameraBufferSetupStep_pushCameraPorjectionValueToGPU(l_camera->CameraDependencies.CameraBufferSetupStep, l_camera->CameraDependencies.SwapChain->SwapChainDependencies.Device);
	};

	void camera_onDetached(void* p_camera, void* p_notUsed)
	{
		Camera* l_camera = (Camera*)p_camera;
		_Utils::Observer_unRegister(&l_camera->CameraDependencies.SwapChain->OnSwapChainBuilded, &l_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, CameraDependencies* p_cameraDependencies)
	{
		p_camera->CameraDependencies = *p_cameraDependencies;
		p_camera->OnSwapChainBuilded.Closure = p_camera;
		p_camera->OnSwapChainBuilded.Callback = camera_onSwapChainBuild;
		_Utils::Observer_register(&p_camera->CameraDependencies.SwapChain->OnSwapChainBuilded, &p_camera->OnSwapChainBuilded);

		p_camera->OnComponentDetached.Closure = p_camera;
		p_camera->OnComponentDetached.Callback = camera_onDetached;
		_Utils::Observer_register(&p_associatedComponent->ComponentFreeEvent, &p_camera->OnComponentDetached);

		p_camera->CameraDependencies.CameraBufferSetupStep->CameraProjection.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		Camera_buildProjectionMatrix(p_camera);
		_Render::CameraBufferSetupStep_pushCameraPorjectionValueToGPU(p_camera->CameraDependencies.CameraBufferSetupStep, p_camera->CameraDependencies.SwapChain->SwapChainDependencies.Device);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		p_camera->CameraDependencies.CameraBufferSetupStep->CameraProjection.Projection = glm::perspective(glm::radians(45.0f),
			p_camera->CameraDependencies.SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->CameraDependencies.SwapChain->SwapChainInfo.SwapExtend.height, 0.1f, 10.0f);
	};
}