#pragma once

#include "v2/Matrix/Matrix.hpp"

namespace _RenderV2
{
	struct Mesh;
}

namespace _MathV2
{
	struct Box;
}

namespace _RenderV2
{
	struct RenderedObject
	{
		Mesh* Mesh;
		_MathV2::Box* MeshBoundingBox;
		_MathV2::Matrix<4, 4, float> ModelMatrix;
	};
}