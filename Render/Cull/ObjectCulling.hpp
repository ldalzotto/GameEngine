#pragma once

namespace _MathV2
{
	struct Box;
	template<int C, int L, typename T>
	struct Matrix;
	struct Frustum;
}

namespace _RenderV2
{
	struct ObjectCullingM
	{
		static bool isObjectCulled(const _MathV2::Box* p_objectBoundingBox_localSpace, const _MathV2::Matrix<4, 4, float>* p_modelMatrix,
			const _MathV2::Matrix<4, 4, float>* p_objectToCamera_matrix, const _MathV2::Frustum* p_cameraFrustum);
	};
}