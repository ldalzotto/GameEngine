#include "ObjectCulling.hpp"

extern "C"
{
#include "v2/_interface/SegmentC.h"
}
#include "v2/Intersection/Intersection.h"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Sphere/Sphere.hpp"
#include "v2/Box/Box.hpp"

using namespace _MathV2;

namespace _RenderV2
{
	bool ObjectCullingM::isObjectCulled(const _MathV2::Box* p_objectBoundingBox_localSpace, const _MathV2::Matrix<4, 4, float>* p_modelMatrix,
			const _MathV2::Matrix<4, 4, float>* p_objectToCamera_matrix, const _MathV2::Frustum* p_cameraFrustum)
	{
		Sphere l_renderBox_asSphere_cameraSpace;
		{
			Vector<4, float> tmp_vec4_0;
			MatrixM::mul(p_objectToCamera_matrix, &VectorM::cast(&p_objectBoundingBox_localSpace->Center, 1.0f), &tmp_vec4_0);
			l_renderBox_asSphere_cameraSpace.Center = *VectorM::cast(&tmp_vec4_0);
		}
		{
			SEGMENT_VECTOR4F l_box_extend_localSpace; l_box_extend_localSpace.Begin = { 0.0f, 0.0f, 0.0f, 1.0f }; 
			l_box_extend_localSpace.End = { p_objectBoundingBox_localSpace->Extend.x, p_objectBoundingBox_localSpace->Extend.y, p_objectBoundingBox_localSpace->Extend.z, 1.0f };
			SEGMENT_VECTOR4F l_box_extend_worldSpace;
			Seg_Mul_V4F_M4F(&l_box_extend_localSpace, (MATRIX4F_PTR)p_modelMatrix, &l_box_extend_worldSpace);
			Vector<4, float> l_box_extend_worldSpace_vec; Seg_ToVector_V4F(&l_box_extend_worldSpace, (VECTOR4F_PTR)&l_box_extend_worldSpace_vec);
			l_renderBox_asSphere_cameraSpace.Radius = VectorM::length(VectorM::cast(&l_box_extend_worldSpace_vec));
		}

		return !Intersection_Contains_Frustum_Sphere(p_cameraFrustum, &l_renderBox_asSphere_cameraSpace);
	};
}