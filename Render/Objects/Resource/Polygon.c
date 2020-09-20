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

void Polygon_Interpolate_V2F(const Polygon2f_PTR p_polygon, float l1, float l2, float l3, Vector2f_PTR out_interpolatedValue)
{
	out_interpolatedValue->x = (p_polygon->v1.x * l1) + (p_polygon->v2.x * l2) + (p_polygon->v3.x * l3);
	out_interpolatedValue->y = (p_polygon->v1.y * l1) + (p_polygon->v2.y * l2) + (p_polygon->v3.y * l3);
};

void Polygon_CalculateNormal_V4FPTR(const Polygon4fPTR_PTR p_polygon, Vector4f_PTR out_normal)
{
	Vec_Min_4f_4f(p_polygon->v1, p_polygon->v2, out_normal);
	Vector3f l_u = out_normal->Vec3;
	Vec_Min_4f_4f(p_polygon->v1, p_polygon->v3, out_normal);
	Vector3f l_v = out_normal->Vec3;
	Vec_Cross_3f(&l_u, &l_v, &out_normal->Vec3);
	out_normal->w = 1.0f;
	Vec_Normalize_3f(&out_normal->Vec3, &out_normal->Vec3);
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