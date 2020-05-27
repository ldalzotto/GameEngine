#pragma once

#include "glm/glm.hpp"

#include "Utils/Observer/Observer.h"
#include "ECS/Component.h"

namespace _GameEngine::_Render
{
	struct Render;
	struct CameraDrawStep;
	struct SwapChain;
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{
		_Render::CameraDrawStep* CameraDrawStep;
		_Render::SwapChain* SwapChain;
		_Utils::Subject OnSwapChainBuilded;
		_Utils::Subject OnComponentDetached;
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::Render* p_render);
	void Camera_buildProjectionMatrix(Camera* p_camera);
}