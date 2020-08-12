#pragma once

#include "v2/Vector/Vector.hpp"

namespace _RenderV2
{
	struct Vertex
	{
		_MathV2::Vector3<float> LocalPosition;
	};

	template<typename T>
	struct VertexVarying
	{
		T Value;
	};
}