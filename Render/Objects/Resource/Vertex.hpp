#pragma once

extern "C"
{
#include "v2/_interface/VectorC_def.h"
}

namespace _RenderV2
{
	struct Vertex
	{
		VECTOR3F LocalPosition;
	};

	template<typename T>
	struct VertexVarying
	{
		T Value;
	};
}