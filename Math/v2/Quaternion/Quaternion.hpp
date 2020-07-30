#pragma once

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

	bool Quaternion_Equals(const Quaternion<float>& p_left, const Quaternion<float>& p_right);
}