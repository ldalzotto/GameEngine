#pragma once

extern "C"
{
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
}

#include "ECS/ComponentT.hpp"

typedef struct RenderV2Interface_TYP RenderV2Interface;

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{
		_ECS::ComponentHeaderT<Camera> ComponentHeader;
		RenderV2Interface* RenderInterface;
		Matrix4f ProjectionMatrix;
		Matrix4f ViewMatrix;
		Frustum CameraFrustum;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<Camera>()
	{
		return &CameraType;
	};

	void Camera_init(Camera* p_camera, RenderV2Interface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);

	Matrix4f Camera_worldToClipMatrix(Camera* p_camera);

	Segment_Vector3f Camera_buildWorldSpaceRay(Camera* p_camera, Vector2f_PTR p_screenPoint);
}