#pragma once

#include "PlaneC_def.h"

void Plane_Build_EquationFactors(const float p_a, const float p_b, const float p_c, const float p_d, PLANE_PTR p_out);
void Plane_Build_Normal_Point(const VECTOR3F_PTR p_normal, const VECTOR3F_PTR p_point, PLANE_PTR p_out);
void  Plane_Build_3Points(const VECTOR3F_PTR p_0, const  VECTOR3F_PTR p_1, const  VECTOR3F_PTR p_2, PLANE_PTR p_out);
float Plane_MinimumDistance_V3F(const PLANE_PTR p_plane, const VECTOR3F_PTR p_point);
void Plane_ProjectPoint(const PLANE_PTR p_plane, const VECTOR3F_PTR p_point, VECTOR3F_PTR out_projectedPoint);