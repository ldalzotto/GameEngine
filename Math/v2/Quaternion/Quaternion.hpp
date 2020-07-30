#pragma once

#include "Functional/Equals/Equals.hpp"

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "RQuaternion.h"

namespace _MathV2
{
	template <typename T>
	struct Quaternion
	{
		T x = 0;
		T y = 0;
		T z = 0;
		T w = 1;
	};

	inline static const Quaternion<float> Quaternionf_Identity = { 0.0f, 0.0f, 0.0f, 1.0f };

	bool Quaternion_Equals(Quaternion<float>& p_left, Quaternion<float>& p_right);
}