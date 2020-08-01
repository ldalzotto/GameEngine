#pragma once

#include "v2/Vector/Vector.hpp"

namespace _GameEngine::_Math
{
	struct Box
	{
		_MathV2::Vector3<float> Center;
		_MathV2::Vector3<float> Extend;
	};

}