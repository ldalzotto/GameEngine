#pragma once

namespace _RenderV2
{
	struct Mesh;
}

namespace _MathV2
{
	template<int C, int L, typename T>
	struct Matrix;

	struct Box;
}

namespace _RenderV2
{
	struct RenderedObject
	{
		Mesh* Mesh;
		_MathV2::Box* MeshBoundingBox;
		_MathV2::Matrix<4, 4, float>* ModelMatrix;
	};
}