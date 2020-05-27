#include "Camera.h"

#include "Render/Render.h"
#include "Render/SwapChain/SwapChain.h"

#include "glm/gtc/matrix_transform.hpp"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "CameraType";

	void camera_onSwapChainBuild(void* p_camera, void* p_swapChain)
	{
		Camera* l_camera = (Camera*)p_camera;
		Camera_buildProjectionMatrix(l_camera);
		_Render::CameraDrawStep_pushCameraPorjectionValueToGPU(l_camera->CameraDrawStep, l_camera->SwapChain->SwapChainDependencies.Device);
	};

	void camera_onDetached(void* p_camera, void* p_notUsed)
	{
		Camera* l_camera = (Camera*)p_camera;
		_Utils::Observer_unRegister(&l_camera->SwapChain->OnSwapChainBuilded, &l_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::Render* p_render)
	{
		p_camera->SwapChain = &p_render->SwapChain;
		p_camera->CameraDrawStep = &p_render->CameraDrawStep;
		p_camera->OnSwapChainBuilded.Closure = p_camera;
		p_camera->OnSwapChainBuilded.Callback = camera_onSwapChainBuild;
		_Utils::Observer_register(&p_camera->SwapChain->OnSwapChainBuilded, &p_camera->OnSwapChainBuilded);

		p_camera->OnComponentDetached.Closure = p_camera;
		p_camera->OnComponentDetached.Callback = camera_onDetached;
		_Utils::Observer_register(&p_associatedComponent->ComponentFreeEvent, &p_camera->OnComponentDetached);

		p_camera->CameraDrawStep->CameraProjection.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		Camera_buildProjectionMatrix(p_camera);
		_Render::CameraDrawStep_pushCameraPorjectionValueToGPU(p_camera->CameraDrawStep, p_camera->SwapChain->SwapChainDependencies.Device);
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		p_camera->CameraDrawStep->CameraProjection.Projection = glm::perspective(glm::radians(45.0f), p_camera->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->SwapChain->SwapChainInfo.SwapExtend.height, 0.1f, 10.0f);
	};
}