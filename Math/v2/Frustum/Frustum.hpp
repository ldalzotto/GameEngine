#pragma once

extern "C"
{
#include "v2/_interface/PlaneC_def.h"
}

namespace _MathV2
{
	struct Frustum
	{
		PLANE Left, Right, Bottom, Up, Near, Far;
	};
}