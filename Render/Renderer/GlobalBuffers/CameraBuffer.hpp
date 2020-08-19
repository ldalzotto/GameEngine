#pragma once

extern "C"
{
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
}
#include "v2/Vector/Vector.hpp"

namespace _RenderV2
{
	struct CameraBuffer
	{
		MATRIX4F_PTR ViewMatrix;
		MATRIX4F_PTR ProjectionMatrix;
		_MathV2::Vector<4, float> WorldPosition;
		FRUSTUM_PTR CameraFrustum;
	};
}