#pragma once

#include "ECS/Component.h"

#include "glm/glm.hpp"

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
		CameraMatrices CameraMatrices;
	};

	void Camera_init(Camera* p_camera);
}