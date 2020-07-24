#pragma once

#include "Math/Matrix/Matrix.h"

#include "Functional/Callback/CallbackT.hpp"
#include "ECS/ComponentT.hpp"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _Math { struct Vector2f; struct Segment; }
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{ 
		_ECS::ComponentHeaderT<Camera> ComponentHeader;
		_Render::RenderInterface* RenderInterface;
		_Math::Matrix4x4f ProjectionMatrix;
		_Math::Matrix4x4f ViewMatrix;
		_Core::CallbackT<Camera, _Render::RenderInterface> OnSwapChainBuilded;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<Camera>()
	{
		return &CameraType;
	};

	void Camera_init(Camera* p_camera, _Render::RenderInterface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);

	void Camera_worldToClipMatrix(Camera* p_camera, _Math::Matrix4x4f* out_worldToClip);

	void Camera_buildWorldSpaceRay(Camera* p_camera, _Math::Vector2f* p_screenPoint, _Math::Segment* out_ray);
}