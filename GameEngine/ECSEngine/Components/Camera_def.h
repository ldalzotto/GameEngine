#pragma once

#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"

#include "Component_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef struct Camera_TYP
{
	ECS_ComponentHeader Header;

	RenderV2Interface* RenderInterface;
	CAMERABUFFER_PTR CameraBuffer;
}Camera, * Camera_PTR;