#pragma once

#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/VectorC_def.h"


typedef struct CAMERABUFFER_TYP
{
	MATRIX4F_PTR ViewMatrix;
	MATRIX4F_PTR ProjectionMatrix;
	VECTOR4F WorldPosition;
	FRUSTUM_PTR CameraFrustum;
}CAMERABUFFER, * CAMERABUFFER_PTR;
