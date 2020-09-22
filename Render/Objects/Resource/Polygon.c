#include "Polygon.h"

#include "v2/_interface/MatrixC.h"
#include "v2/_interface/RectC.h"
#include "v2/_interface/VectorC.h"

void Polygon_Mul_V4F_M4F(const Polygon4f_PTR p_polygon, const Matrix4f_PTR p_matrix, Polygon4f_PTR out_polygon)
{
	Mat_Mul_M4F_V4F(p_matrix, &p_polygon->v1, &out_polygon->v1);
	Mat_Mul_M4F_V4F(p_matrix, &p_polygon->v2, &out_polygon->v2);
	Mat_Mul_M4F_V4F(p_matrix, &p_polygon->v3, &out_polygon->v3);
};

void Polygon_MulHomogeneous_V4F_M4F(const Polygon4f_PTR p_polygon, const Matrix4f_PTR p_matrix, Polygon4f_PTR out_polygon)
{
	Mat_Mul_M4F_V4F_Homogeneous(p_matrix, &p_polygon->v1, &out_polygon->v1);
	Mat_Mul_M4F_V4F_Homogeneous(p_matrix, &p_polygon->v2, &out_polygon->v2);
	Mat_Mul_M4F_V4F_Homogeneous(p_matrix, &p_polygon->v3, &out_polygon->v3);
};

void Polygon_CalculateNormal_V3FPTR(const Polygon4fPTR_PTR p_polygon, Vector3f_PTR out_normal)
{
	Vec_Min_3f_3f(&p_polygon->v1->Vec3, &p_polygon->v2->Vec3, out_normal);
	Vector3f l_u = *out_normal;
	Vec_Min_3f_3f(&p_polygon->v1->Vec3, &p_polygon->v3->Vec3, out_normal);
	Vector3f l_v = *out_normal;
	Vec_Cross_3f(&l_u, &l_v, out_normal);
	Vec_Normalize_3f(out_normal, out_normal);
};

void Polygon_BoundingRect_2i(const Polygon2i_PTR p_polygon, Recti_PTR out_boudingRect)
{
	*out_boudingRect = (Recti) {
		.Min = p_polygon->v1,
		.Max = p_polygon->v1
	};
	for (short int i = 1; i < 3; i++)
	{
		if (p_polygon->Points[i].x < out_boudingRect->Min.x) { out_boudingRect->Min.x = p_polygon->Points[i].x; }
		if (p_polygon->Points[i].x > out_boudingRect->Max.x) { out_boudingRect->Max.x = p_polygon->Points[i].x; }
		if (p_polygon->Points[i].y < out_boudingRect->Min.y) { out_boudingRect->Min.y = p_polygon->Points[i].y; }
		if (p_polygon->Points[i].y > out_boudingRect->Max.y) { out_boudingRect->Max.y = p_polygon->Points[i].y; }
	}
};