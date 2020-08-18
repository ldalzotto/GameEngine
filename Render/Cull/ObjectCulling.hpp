#pragma once

extern "C"
{
#include "v2/_interface/BoxC_def.h"
}

namespace _MathV2
{
	template<int C, int L, typename T>
	struct Matrix;
	struct Frustum;
}

namespace _RenderV2
{
	struct ObjectCullingM
	{
		static bool isObjectCulled(const BOXF_PTR p_objectBoundingBox_localSpace, const _MathV2::Matrix<4, 4, float>* p_modelMatrix,
			const _MathV2::Matrix<4, 4, float>* p_objectToCamera_matrix, const _MathV2::Frustum* p_cameraFrustum);
	};
}