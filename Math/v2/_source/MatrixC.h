#pragma once

#include "MatrixC_def.h"
#include "QuaternionC_def.h"

const MATRIX4F MATRIX4F_IDENTITYF = { 1.0f, 0.0f, 0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

void Mat_Mul_M4F_M4F(const MATRIX4F_PTR p_left, const MATRIX4F_PTR p_right, MATRIX4F_PTR p_out);
void Mat_Mul_M4F_V4F(const MATRIX4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out);
void Mat_Mul_M3F_V3F(const MATRIX3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out);

void Mat_Inv_M4F(const MATRIX4F_PTR p_matrix, MATRIX4F_PTR p_out);

void Mat_Translation_M4F(const VECTOR3F_PTR p_translation, MATRIX4F_PTR out_mat);
void Mat_RotationAxis_M4F(const MATRIX3F_PTR p_axis, MATRIX4F_PTR out_mat);
void Mat_RotationSeparatedAxis_M4F(const VECTOR3F_PTR p_right, const VECTOR3F_PTR p_up, const VECTOR3F_PTR p_forward, MATRIX4F_PTR out_mat);
void Mat_Scale_M4F(const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_mat);

void Mat_TRS_Axis_M4F(const VECTOR3F_PTR p_position, const MATRIX3F_PTR p_axis, const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_TRS);
void Mat_TRS_Quat_M4F(const VECTOR3F_PTR p_position, const QUATERNION4F_PTR p_quat, const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_TRS);
