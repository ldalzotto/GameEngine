#pragma once

#include "Frustum.hpp"
#include "v2/Matrix/Matrix.hpp"

namespace _MathV2
{
	struct FrustumM
	{
		static Frustum extractFrustumFromProjection(Matrix<4, 4, float>* p_projection);
	};
}