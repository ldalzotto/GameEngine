#include "ObjectCulling.hpp"

#include "v2/Intersection/Intersection.h"
#include "v2/Segment/SegmentV2Math.hpp"
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
			SegmentV2<4, float> l_box_extend_localSpace = { {0.0f, 0.0f, 0.0f, 1.0f}, VectorM::cast(&p_objectBoundingBox_localSpace->Extend, 1.0f) };
			SegmentV2<4, float> l_box_extend_worldSpace;
			SegmentM::mul(&l_box_extend_localSpace, p_modelMatrix, &l_box_extend_worldSpace);
			Vector<4, float> l_box_extend_worldSpace_vec; SegmentM::toVector(&l_box_extend_worldSpace, &l_box_extend_worldSpace_vec);
			l_renderBox_asSphere_cameraSpace.Radius = VectorM::length(VectorM::cast(&l_box_extend_worldSpace_vec));
		}

		return !Intersection_Contains_Frustum_Sphere(p_cameraFrustum, &l_renderBox_asSphere_cameraSpace);
	};
}