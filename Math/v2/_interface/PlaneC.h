#pragma once

#include "PlaneC_def.h"

void Plane_Build_EquationFactors(const float p_a, const float p_b, const float p_c, const float p_d, Plane_PTR p_out);
void Plane_Build_Normal_Point(const Vector3f_PTR p_normal, const Vector3f_PTR p_point, Plane_PTR p_out);
void  Plane_Build_3Points(const Vector3f_PTR p_0, const  Vector3f_PTR p_1, const  Vector3f_PTR p_2, Plane_PTR p_out);
float Plane_MinimumDistance_V3F(const Plane_PTR p_plane, const Vector3f_PTR p_point);
void Plane_ProjectPoint(const Plane_PTR p_plane, const Vector3f_PTR p_point, Vector3f_PTR out_projectedPoint);