#include "Camera.h"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "CameraType";

	void Camera_init(Camera* p_camera)
	{
		/*
		p_camera->CameraMatrices.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		p_camera->CameraMatrices.Projection = glm::perspective(glm::radians(45.0f), p_meshDrawSystem->Render->SwapChain.SwapChainInfo.SwapExtend.width / (float)p_meshDrawSystem->Render->SwapChain.SwapChainInfo.SwapExtend.height, 0.1f, 10.0f);
		p_camera->CameraMatrices.Projection[1][1] *= -1;
		*/
	};
}