#pragma once

#include "Objects/Resource/Mesh.hpp"
#include "v2/Box/Box.hpp"

namespace _MathV2
{
	template<int C, int L, typename T>
	struct Matrix;
}

namespace _RenderV2
{
	struct RenderedObject
	{
		Mesh Mesh;
		_MathV2::Box MeshBoundingBox;
		_MathV2::Matrix<4, 4, float>* ModelMatrix;
	};
}