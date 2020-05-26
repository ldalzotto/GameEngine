#pragma once

#include "ECS/ECS.h"
#include "Render/Render.h"

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{
		_Render::Render* Render;
		glm::mat4x4 View;
		glm::mat4x4 Projection;
	};

}