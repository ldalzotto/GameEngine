#pragma once

#include "v2/Matrix/Matrix.hpp"

#include "Functional/Callback/CallbackT.hpp"
#include "ECS/ComponentT.hpp"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _Math { struct Segment; }
}

namespace _MathV2
{
	template <int N, typename T>
	struct Vector;
}

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{ 
		_ECS::ComponentHeaderT<Camera> ComponentHeader;
		_Render::RenderInterface* RenderInterface;
		_MathV2::Matrix4x4<float> ProjectionMatrix;
		_MathV2::Matrix4x4<float> ViewMatrix;
		_Core::CallbackT<Camera, _Render::RenderInterface> OnSwapChainBuilded;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<Camera>()
	{
		return &CameraType;
	};

	void Camera_init(Camera* p_camera, _Render::RenderInterface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);

	_MathV2::Matrix4x4<float> Camera_worldToClipMatrix(Camera* p_camera);

	_Math::Segment& Camera_buildWorldSpaceRay(Camera* p_camera, _MathV2::Vector<2, float>& p_screenPoint, _Math::Segment&& out_ray);
}