#pragma once

#include "Math/Matrix/Matrix.h"

#include "Functional/Callback/CallbackT.hpp"
#include "ECS/ComponentType.h"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _ECS { struct Component; }
	namespace _Math { struct Vector2f; }
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{
		_Render::RenderInterface* RenderInterface;
		_Math::Matrix4x4f ProjectionMatrix;
		_Math::Matrix4x4f ViewMatrix;
		_Core::CallbackT<Camera, _Render::RenderInterface> OnSwapChainBuilded;
		_Core::CallbackT<Camera, void> OnComponentDetached;
	};

	void Camera_init(Camera* p_camera, Component* p_associatedComponent, _Render::RenderInterface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);
}