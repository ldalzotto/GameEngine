#pragma once

#include "Polygon_def.h"
#include "v2/_interface/MatrixC_def.h"

void Polygon_Mul_V4F_M4F(const POLYGON4F_PTR p_polygon, const MATRIX4F_PTR p_matrix, POLYGON4F_PTR out_polygon);
void Polygon_MulHomogeneous_V4F_M4F(const POLYGON4F_PTR p_polygon, const MATRIX4F_PTR p_matrix, POLYGON4F_PTR out_polygon);