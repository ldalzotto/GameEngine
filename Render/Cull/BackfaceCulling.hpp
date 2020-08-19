#pragma once

extern "C"
{
#include "v2/_interface/VectorC_def.h"
#include "Objects/Resource/Polygon_def.h"
}

namespace _RenderV2
{
	struct BackfaceCullingM
	{
		static bool isCulled(const POLYGON4F_PTR p_polygon, const VECTOR4F_PTR p_cameraWorldPosition);
	};
}