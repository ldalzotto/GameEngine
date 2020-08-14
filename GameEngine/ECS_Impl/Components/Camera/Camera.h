#pragma once

#include "v2/Matrix/Matrix.hpp"
#include "v2/Segment/SegmentV2.hpp"
#include "v2/Frustum/Frustum.hpp"

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
		_MathV2::Frustum CameraFrustum;
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

	_MathV2::SegmentV2<3, float> Camera_buildWorldSpaceRay(Camera* p_camera, _MathV2::Vector<2, float>& p_screenPoint);
}