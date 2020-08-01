#pragma once

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	struct Box
	{
		_MathV2::Vector3<float> Center;
		_MathV2::Vector3<float> Extend;
	};

}