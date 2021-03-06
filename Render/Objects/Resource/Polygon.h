#pragma once

#include "Polygon_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/RectC_def.h"

void Polygon_Mul_V4F_M4F(const Polygon4f_PTR p_polygon, const Matrix4f_PTR p_matrix, Polygon4f_PTR out_polygon);
void Polygon_MulHomogeneous_V4F_M4F(const Polygon4f_PTR p_polygon, const Matrix4f_PTR p_matrix, Polygon4f_PTR out_polygon);

void Polygon_CalculateNormal_V3FPTR(const Polygon4fPTR_PTR p_polygon, Vector3f_PTR out_normal);
void Polygon_BoundingRect_2i(const Polygon2i_PTR p_polygon, Recti_PTR out_boudingRect);