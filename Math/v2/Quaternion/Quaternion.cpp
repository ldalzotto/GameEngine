#include "Quaternion.hpp"

namespace _MathV2
{
	bool Quaternion_Equals(const Quaternion<float>& p_left, const Quaternion<float>& p_right)
	{
		return _Core::Equals_float(&p_left.x, &p_right.x)
			&& _Core::Equals_float(&p_left.y, &p_right.y)
			&& _Core::Equals_float(&p_left.z, &p_right.z);
	};
}