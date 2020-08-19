#pragma once

extern "C"
{
#include "v2/_interface/FrustumC_def.h"
}
#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	template<int C, int L, typename T>
	struct Matrix;
}

namespace _RenderV2
{
	struct CameraBuffer
	{
		_MathV2::Matrix<4, 4, float>* ViewMatrix;
		_MathV2::Matrix<4, 4, float>* ProjectionMatrix;
		_MathV2::Vector<4, float> WorldPosition;
		FRUSTUM_PTR CameraFrustum;
	};
}