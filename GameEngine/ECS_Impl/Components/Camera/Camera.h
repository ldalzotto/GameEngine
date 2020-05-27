#pragma once

#include "glm/glm.hpp"

#include "Utils/Observer/Observer.h"
#include "ECS/Component.h"


namespace _GameEngine::_Render
{
	struct SwapChain;
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct CameraMatrices
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};

	struct Camera
	{
		_Render::SwapChain* SwapChain;
		_Utils::Subject OnSwapChainBuilded;
		CameraMatrices CameraMatrices;

		_Utils::Subject OnComponentDetached;
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::SwapChain* p_swapChain);
	void Camera_buildProjectionMatrix(Camera* p_camera);
}