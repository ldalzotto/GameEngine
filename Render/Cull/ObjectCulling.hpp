#pragma once

extern "C"
{
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
}


namespace _RenderV2
{
	struct ObjectCullingM
	{
		static bool isObjectCulled(const BOXF_PTR p_objectBoundingBox_localSpace, const MATRIX4F_PTR p_modelMatrix,
			const MATRIX4F_PTR p_objectToCamera_matrix, const FRUSTUM_PTR p_cameraFrustum);
	};
}