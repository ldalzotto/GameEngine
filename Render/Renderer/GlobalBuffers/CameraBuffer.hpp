#pragma once

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	template<int C, int L, typename T>
	struct Matrix;
	struct Frustum;
}

namespace _RenderV2
{
	struct CameraBuffer
	{
		_MathV2::Matrix<4, 4, float>* ViewMatrix;
		_MathV2::Matrix<4, 4, float>* ProjectionMatrix;
		_MathV2::Vector<4, float> WorldPosition;
		_MathV2::Frustum* CameraFrustum;
	};
}