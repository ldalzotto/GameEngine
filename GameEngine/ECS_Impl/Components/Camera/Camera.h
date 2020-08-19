#pragma once

#include "v2/Matrix/Matrix.hpp"
extern "C"
{
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/FrustumC_def.h"
}

#include "Functional/Callback/CallbackT.hpp"
#include "ECS/ComponentT.hpp"

namespace _MathV2
{
	template <int N, typename T>
	struct Vector;
}

namespace _RenderV2
{
	struct RenderV2Interface;
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{
		_ECS::ComponentHeaderT<Camera> ComponentHeader;
		_RenderV2::RenderV2Interface* RenderInterface;
		_MathV2::Matrix4x4<float> ProjectionMatrix;
		_MathV2::Matrix4x4<float> ViewMatrix;
		FRUSTUM CameraFrustum;
		_Core::CallbackT<Camera, _RenderV2::RenderV2Interface> OnSwapChainBuilded;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<Camera>()
	{
		return &CameraType;
	};

	void Camera_init(Camera* p_camera, _RenderV2::RenderV2Interface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);

	_MathV2::Matrix4x4<float> Camera_worldToClipMatrix(Camera* p_camera);

	SEGMENT_VECTOR3F Camera_buildWorldSpaceRay(Camera* p_camera, VECTOR2F_PTR p_screenPoint);
}