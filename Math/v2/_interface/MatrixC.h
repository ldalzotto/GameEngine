#pragma once

#include "MatrixC_def.h"
#include "v2/_interface/QuaternionC_def.h"

extern const MATRIX4F MATRIX4F_IDENTITYF;

void Mat_Mul_M4F_M4F(const MATRIX4F_PTR p_left, const MATRIX4F_PTR p_right, MATRIX4F_PTR p_out);
void Mat_Mul_M4F_V4F(const MATRIX4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out);
void Mat_Mul_M4F_V4F_Homogeneous(const MATRIX4F_PTR p_projectionmatrix, const VECTOR4F_PTR p_pos, VECTOR4F_PTR out_pos);
void Mat_Mul_M3F_V3F(const MATRIX3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out);

void Mat_Inv_M4F(const MATRIX4F_PTR p_matrix, MATRIX4F_PTR p_out);

void Mat_Translation_M4F(const VECTOR3F_PTR p_translation, MATRIX4F_PTR out_mat);
void Mat_RotationAxis_M4F(const MATRIX3F_PTR p_axis, MATRIX4F_PTR out_mat);
void Mat_RotationSeparatedAxis_M4F(const VECTOR3F_PTR p_right, const VECTOR3F_PTR p_up, const VECTOR3F_PTR p_forward, MATRIX4F_PTR out_mat);
void Mat_Scale_M4F(const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_mat);

void Mat_TRS_Axis_M4F(const VECTOR3F_PTR p_position, const MATRIX3F_PTR p_axis, const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_TRS);
void Mat_TRS_Quat_M4F(const VECTOR3F_PTR p_position, const QUATERNION4F_PTR p_quat, const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_TRS);

void Mat_GetTranslation_M4F(const MATRIX4F_PTR p_mat, VECTOR4F_PTR out_translation);
void Mat_GetScale_M4F(const MATRIX4F_PTR p_trs, VECTOR4F_PTR out_scale);

void Mat_Perspective_M4F(const float p_fov, const float p_aspect, const float p_near, const float p_far, MATRIX4F_PTR p_out);
void Mat_LookAtRotation_F(const VECTOR3F_PTR p_origin, const VECTOR3F_PTR p_target, const VECTOR3F_PTR p_up, MATRIX3F_PTR out_rotationMatrix);