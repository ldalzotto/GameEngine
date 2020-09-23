#pragma once

#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/VectorC_def.h"


typedef struct CAMERABUFFER_TYP
{
	Matrix4f ViewMatrix;
	Matrix4f ProjectionMatrix;
	Vector4f WorldPosition;
	float Far;
	Frustum CameraFrustum;
}CAMERABUFFER, * CAMERABUFFER_PTR;
