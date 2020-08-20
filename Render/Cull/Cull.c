#include "BackfaceCulling.h"
#include "ObjectCulling.h"

#include "Constants.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/Intersection.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"

bool BackFaceCulled_Poly4F(const POLYGON4F_PTR p_polygon, const VECTOR4F_PTR p_cameraWorldPosition)
{
	VECTOR4F tmp_vec4_0;

	Vec_Min_4f_4f(&p_polygon->v1, p_cameraWorldPosition, &tmp_vec4_0);
	VECTOR3F l_cameraToPolygon = tmp_vec4_0.Vec3;
	Vec_Min_4f_4f(&p_polygon->v1, &p_polygon->v2, &tmp_vec4_0);
	VECTOR3F l_u = tmp_vec4_0.Vec3;
	Vec_Min_4f_4f(&p_polygon->v1, &p_polygon->v3, &tmp_vec4_0);
	VECTOR3F l_v = tmp_vec4_0.Vec3;
	VECTOR3F l_n;
	Vec_Cross_3f(&l_u, &l_v, &l_n);

	return Vec_Dot_3f(&l_cameraToPolygon, &l_n) > FLOAT_TOLERANCE;
};

bool ObjectCulled_Boxf(const BOXF_PTR p_objectBoundingBox_localSpace, const MATRIX4F_PTR p_modelMatrix,
	const MATRIX4F_PTR p_objectToCamera_matrix, const FRUSTUM_PTR p_cameraFrustum)
{
	SPHEREF l_renderBox_asSphere_cameraSpace;
	{
		VECTOR4F tmp_vec4_0, tmp_vec4_1;
		tmp_vec4_0 = (VECTOR4F) { p_objectBoundingBox_localSpace->Center.x, p_objectBoundingBox_localSpace->Center.y, p_objectBoundingBox_localSpace->Center.z, 1.0f };
		Mat_Mul_M4F_V4F(p_objectToCamera_matrix, &tmp_vec4_0, &tmp_vec4_1);
		l_renderBox_asSphere_cameraSpace.Center = tmp_vec4_1.Vec3;
	}
	{
		SEGMENT_VECTOR4F l_box_extend_localSpace = { .Begin = {0.0f, 0.0f, 0.0f, 1.0f}, .End = { p_objectBoundingBox_localSpace->Extend.x, p_objectBoundingBox_localSpace->Extend.y, p_objectBoundingBox_localSpace->Extend.z, 1.0f } };
		SEGMENT_VECTOR4F l_box_extend_worldSpace;
		Seg_Mul_V4F_M4F(&l_box_extend_localSpace, (MATRIX4F_PTR)p_modelMatrix, &l_box_extend_worldSpace);
		VECTOR4F l_box_extend_worldSpace_vec; Seg_ToVector_V4F(&l_box_extend_worldSpace, (VECTOR4F_PTR)&l_box_extend_worldSpace_vec);
		l_renderBox_asSphere_cameraSpace.Radius = Vec_Length_3f(&l_box_extend_worldSpace_vec.Vec3);
	}

	return !Intersection_Contains_Frustum_Sphere(p_cameraFrustum, &l_renderBox_asSphere_cameraSpace);
};