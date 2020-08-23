#pragma once

#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"

#include "Component_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

// extern ComponentType CameraType;

typedef struct Camera_TYP
{
	ECS_ComponentHeader Header;

	RenderV2Interface* RenderInterface;
	Matrix4f ProjectionMatrix;
	Matrix4f ViewMatrix;
	Frustum CameraFrustum;
}Camera, * Camera_PTR;