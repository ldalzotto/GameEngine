#pragma once

#include "QuaternionC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/MatrixC_def.h"

#include <stdbool.h>

extern const QUATERNION4F QUATERNION4F_IDENTITY;

bool Quat_Equals(const QUATERNION4F_PTR p_left, const QUATERNION4F_PTR p_right);

void Quat_Normalize(const QUATERNION4F_PTR p_quat, QUATERNION4F_PTR p_out);
void Quat_Mul(const QUATERNION4F_PTR p_left, const QUATERNION4F_PTR p_right, QUATERNION4F_PTR p_out);
void Quat_Cross(const QUATERNION4F_PTR p_left, const  QUATERNION4F_PTR p_right, QUATERNION4F_PTR p_out);

void Quat_Build_4f(const VECTOR4F_PTR p_points, QUATERNION4F_PTR p_out);
void Quat_Build_3f_1f(const VECTOR3F_PTR p_vec, float p_scal, QUATERNION4F_PTR p_out);
void Quat_FromDirection(const VECTOR3F_PTR p_vec, QUATERNION4F_PTR p_out);
void Quat_RotateAround(const VECTOR3F_PTR p_axis, const  float p_angle, QUATERNION4F_PTR out_quat);
void Quat_ExtractAxis(const QUATERNION4F_PTR quat, MATRIX3F_PTR out_axis);
void Quat_FromAxis(const float p_axis[3][3], QUATERNION4F_PTR p_out);
void Quat_FromEulerAngle(const VECTOR3F_PTR p_eulerAngle, QUATERNION4F_PTR p_out);
void Quat_FromTo(const VECTOR3F_PTR p_from, const  VECTOR3F_PTR p_to, QUATERNION4F_PTR p_out);
void Quat_conjugate(const QUATERNION4F_PTR p_quat, QUATERNION4F_PTR p_out);