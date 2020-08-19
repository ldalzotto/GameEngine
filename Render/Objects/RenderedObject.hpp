#pragma once

extern "C"
{
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "Objects/Resource/Mesh_def.h"
}

namespace _RenderV2
{
	struct RenderedObject
	{
		MESH_PTR Mesh;
		BOXF_PTR MeshBoundingBox;
		MATRIX4F ModelMatrix;
	};
}