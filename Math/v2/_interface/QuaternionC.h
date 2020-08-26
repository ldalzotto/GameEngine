#pragma once

#include "QuaternionC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/MatrixC_def.h"

#include <stdbool.h>

extern const Quaternion4f Quaternion4f_IDENTITY;

bool Quat_Equals(const Quaternion4f_PTR p_left, const Quaternion4f_PTR p_right);

void Quat_Normalize(const Quaternion4f_PTR p_quat, Quaternion4f_PTR p_out);
void Quat_Mul(const Quaternion4f_PTR p_left, const Quaternion4f_PTR p_right, Quaternion4f_PTR p_out);
void Quat_Cross(const Quaternion4f_PTR p_left, const  Quaternion4f_PTR p_right, Quaternion4f_PTR p_out);

void Quat_Build_4f(const Vector4f_PTR p_points, Quaternion4f_PTR p_out);
void Quat_Build_3f_1f(const Vector3f_PTR p_vec, float p_scal, Quaternion4f_PTR p_out);
void Quat_FromDirection(const Vector3f_PTR p_vec, Quaternion4f_PTR p_out);
void Quat_RotateAround(const Vector3f_PTR p_axis, const  float p_angle, Quaternion4f_PTR out_quat);
void Quat_ExtractAxis(const Quaternion4f_PTR quat, Matrix3f_PTR out_axis);
void Quat_FromAxis(const Matrix3f_PTR p_axis, Quaternion4f_PTR p_out);
void Quat_FromEulerAngle(const Vector3f_PTR p_eulerAngle, Quaternion4f_PTR p_out);
void Quat_FromTo(const Vector3f_PTR p_from, const  Vector3f_PTR p_to, Quaternion4f_PTR p_out);
void Quat_conjugate(const Quaternion4f_PTR p_quat, Quaternion4f_PTR p_out);