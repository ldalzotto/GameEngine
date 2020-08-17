#pragma once

#include "QuaternionC_def.h"
#include "VectorC_def.h"

void Quat_Normalize(const QUATERNION4F_PTR p_quat, QUATERNION4F_PTR p_out);
void Quat_Mul(const QUATERNION4F_PTR p_left, const QUATERNION4F_PTR p_right, QUATERNION4F_PTR p_out);
void Quat_Cross(const QUATERNION4F_PTR p_left, const  QUATERNION4F_PTR p_right, QUATERNION4F_PTR p_out);

void Quat_Build(const VECTOR3F_PTR p_vec, float p_scal, QUATERNION4F_PTR p_out);
void Quat_FromDirection(const VECTOR3F_PTR p_vec, QUATERNION4F_PTR p_out);
void Quat_RotateAround(const VECTOR3F_PTR p_axis, const  float p_angle, QUATERNION4F_PTR out_quat);
void Quat_ExtractAxis(const QUATERNION4F_PTR quat, float out_axis[3][3]);
void Quat_FromAxis(const float p_axis[3][3], QUATERNION4F_PTR p_out);
void Quat_FromEulerAngle(const VECTOR3F_PTR p_eulerAngle, QUATERNION4F_PTR p_out);
void Quat_FromTo(const VECTOR3F_PTR p_from, const  VECTOR3F_PTR p_to, QUATERNION4F_PTR p_out);
void Quat_conjugate(const QUATERNION4F_PTR p_quat, QUATERNION4F_PTR p_out);