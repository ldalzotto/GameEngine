#include "BackfaceCulling.h"
#include "ObjectCulling.h"

#include "Constants.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/Intersection.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"

bool BackFaceCulled_Poly4F(const Polygon4f_PTR p_polygon, const Vector4f_PTR p_cameraWorldPosition)
{
	Vector4f tmp_vec4_0;

	Vec_Min_4f_4f(&p_polygon->v1, p_cameraWorldPosition, &tmp_vec4_0);
	Vector3f l_cameraToPolygon = tmp_vec4_0.Vec3;
	Vec_Min_4f_4f(&p_polygon->v1, &p_polygon->v2, &tmp_vec4_0);
	Vector3f l_u = tmp_vec4_0.Vec3;
	Vec_Min_4f_4f(&p_polygon->v1, &p_polygon->v3, &tmp_vec4_0);
	Vector3f l_v = tmp_vec4_0.Vec3;
	Vector3f l_n;
	Vec_Cross_3f(&l_u, &l_v, &l_n);

	return Vec_Dot_3f(&l_cameraToPolygon, &l_n) > FLOAT_TOLERANCE;
};

bool ObjectCulled_Boxf(const BoxF_PTR p_objectBoundingBox_localSpace, const Matrix4f_PTR p_modelMatrix,
	const Matrix4f_PTR p_objectToCamera_matrix, const Frustum_PTR p_cameraFrustum)
{
	Spheref l_renderBox_asSphere_cameraSpace;
	{
		Vector4f tmp_vec4_0, tmp_vec4_1;
		tmp_vec4_0 = (Vector4f) { p_objectBoundingBox_localSpace->Center.x, p_objectBoundingBox_localSpace->Center.y, p_objectBoundingBox_localSpace->Center.z, 1.0f };
		Mat_Mul_M4F_V4F(p_objectToCamera_matrix, &tmp_vec4_0, &tmp_vec4_1);
		l_renderBox_asSphere_cameraSpace.Center = tmp_vec4_1.Vec3;
	}
	{
		Segment_Vector4f l_box_extend_localSpace = { .Begin = {0.0f, 0.0f, 0.0f, 1.0f}, .End = { p_objectBoundingBox_localSpace->Extend.x, p_objectBoundingBox_localSpace->Extend.y, p_objectBoundingBox_localSpace->Extend.z, 1.0f } };
		Segment_Vector4f l_box_extend_worldSpace;
		Seg_Mul_V4F_M4F(&l_box_extend_localSpace, (Matrix4f_PTR)p_modelMatrix, &l_box_extend_worldSpace);
		Vector4f l_box_extend_worldSpace_vec; Seg_ToVector_V4F(&l_box_extend_worldSpace, (Vector4f_PTR)&l_box_extend_worldSpace_vec);
		l_renderBox_asSphere_cameraSpace.Radius = Vec_Length_3f(&l_box_extend_worldSpace_vec.Vec3);
	}

	return !Intersection_Contains_Frustum_Sphere(p_cameraFrustum, &l_renderBox_asSphere_cameraSpace);
};