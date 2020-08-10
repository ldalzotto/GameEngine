#pragma once

#include "v2/Plane/Plane.hpp"

namespace _MathV2
{
	struct Frustum
	{
		Plane Left, Right, Bottom, Up, Near, Far;
	};
}