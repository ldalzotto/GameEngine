#pragma once

extern "C"
{
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/MatrixC_def.h"
}


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
		MATRIX4F ModelMatrix;
	};
}