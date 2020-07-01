#pragma once

#include "Math/Matrix/Matrix.h"

#include "Utils/Observer/Observer.h"
#include "ECS/ComponentType.h"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _ECS { struct Component; }
	namespace _Math { struct Vector2f; }
}

namespace _GameEngine::_ECS
{
	struct Camera
	{
		_Render::RenderInterface* RenderInterface;
		_Math::Matrix4x4f ProjectionMatrix;
		_Math::Matrix4x4f ViewMatrix;
		_Utils::Subject OnSwapChainBuilded;
		_Utils::Subject OnComponentDetached;
	};

	template<>
	extern inline ComponentType* extractComponentType<Camera>();

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::RenderInterface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);
}