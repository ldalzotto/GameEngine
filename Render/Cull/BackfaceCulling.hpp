#pragma once

extern "C"
{
#include "v2/_interface/VectorC_def.h"
}

namespace _RenderV2
{
	template<typename T>
	struct Polygon;
}

namespace _RenderV2
{
	struct BackfaceCullingM
	{
		static bool isCulled(const Polygon<VECTOR4F>* p_polygon, const VECTOR4F_PTR p_cameraWorldPosition);
	};
}