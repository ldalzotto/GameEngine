#pragma once

#include "glm/glm.hpp"

#include "Utils/Observer/Observer.h"
#include "ECS/ComponentType.h"

namespace _GameEngine
{
	namespace _Render
	{
		struct SwapChain;
	}
	
	namespace _ECS
	{
		struct Component;
	}
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct CameraDependencies
	{
		_Render::SwapChain* SwapChain;
	};

	struct Camera
	{
		CameraDependencies CameraDependencies;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		_Utils::Subject OnSwapChainBuilded;
		_Utils::Subject OnComponentDetached;
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, CameraDependencies* p_cameraDependencies);
	void Camera_buildProjectionMatrix(Camera* p_camera);
}