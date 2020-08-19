#include "Polygon.h"

#include "v2/_interface/MatrixC.h"

void Polygon_Mul_V4F_M4F(const POLYGON4F_PTR p_polygon, const MATRIX4F_PTR p_matrix, POLYGON4F_PTR out_polygon)
{
	Mat_Mul_M4F_V4F(p_matrix, &p_polygon->v1, &out_polygon->v1);
	Mat_Mul_M4F_V4F(p_matrix, &p_polygon->v2, &out_polygon->v2);
	Mat_Mul_M4F_V4F(p_matrix, &p_polygon->v3, &out_polygon->v3);
};
void Polygon_MulHomogeneous_V4F_M4F(const POLYGON4F_PTR p_polygon, const MATRIX4F_PTR p_matrix, POLYGON4F_PTR out_polygon)
{
	Mat_Mul_M4F_V4F_Homogeneous(p_matrix, &p_polygon->v1, &out_polygon->v1);
	Mat_Mul_M4F_V4F_Homogeneous(p_matrix, &p_polygon->v2, &out_polygon->v2);
	Mat_Mul_M4F_V4F_Homogeneous(p_matrix, &p_polygon->v3, &out_polygon->v3);
};