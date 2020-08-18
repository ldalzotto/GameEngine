#pragma once

extern "C"
{
#include "v2/_interface/BoxC_def.h"
}

#include "v2/Matrix/Matrix.hpp"

namespace _RenderV2
{
	struct Mesh;
}

namespace _RenderV2
{
	struct RenderedObject
	{
		Mesh* Mesh;
		BOXF_PTR MeshBoundingBox;
		_MathV2::Matrix<4, 4, float> ModelMatrix;
	};
}