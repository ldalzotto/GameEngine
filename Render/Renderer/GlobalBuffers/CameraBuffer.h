#pragma once

#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/VectorC_def.h"


typedef struct CAMERABUFFER_TYP
{
	Matrix4f_PTR ViewMatrix;
	Matrix4f_PTR ProjectionMatrix;
	Vector4f WorldPosition;
	Frustum_PTR CameraFrustum;
}CAMERABUFFER, * CAMERABUFFER_PTR;
