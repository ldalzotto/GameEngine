#pragma once

#include "MatrixC_def.h"
#include "v2/_interface/QuaternionC_def.h"

extern const Matrix4f Matrix4f_IDENTITYF;

void Mat_Mul_M4F_M4F(const Matrix4f_PTR p_left, const Matrix4f_PTR p_right, Matrix4f_PTR p_out);
void Mat_Mul_M4F_V4F(const Matrix4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out);
void Mat_Mul_M4F_V4F_Homogeneous(const Matrix4f_PTR p_projectionmatrix, const Vector4f_PTR p_pos, Vector4f_PTR out_pos);
void Mat_Mul_M3F_M3F(const Matrix3f_PTR p_left, const Matrix3f_PTR p_right, Matrix3f_PTR p_out);
void Mat_Mul_M3F_V3F(const Matrix3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out);
void Mat_Mul_M3F_1F(const Matrix3f_PTR p_left, const float p_right, Matrix3f_PTR p_out);

void Mat_Inv_M4F(const Matrix4f_PTR p_matrix, Matrix4f_PTR p_out);

void Mat_Translation_M4F(const Vector3f_PTR p_translation, Matrix4f_PTR out_mat);
void Mat_RotationAxis_M4F(const Matrix3f_PTR p_axis, Matrix4f_PTR out_mat);
void Mat_RotationSeparatedAxis_M4F(const Vector3f_PTR p_right, const Vector3f_PTR p_up, const Vector3f_PTR p_forward, Matrix4f_PTR out_mat);
void Mat_Scale_M4F(const Vector3f_PTR p_scale, Matrix4f_PTR out_mat);

void Mat_TRS_Axis_M4F(const Vector3f_PTR p_position, const Matrix3f_PTR p_axis, const Vector3f_PTR p_scale, Matrix4f_PTR out_TRS);
void Mat_TRS_Quat_M4F(const Vector3f_PTR p_position, const Quaternion4f_PTR p_quat, const Vector3f_PTR p_scale, Matrix4f_PTR out_TRS);

void Mat_GetTranslation_M4F(const Matrix4f_PTR p_mat, Vector4f_PTR out_translation);
void Mat_GetScale_M4F(const Matrix4f_PTR p_trs, Vector4f_PTR out_scale);

void Mat_Perspective_M4F(const float p_fov, const float p_aspect, const float p_near, const float p_far, Matrix4f_PTR p_out);
void Mat_LookAtRotation_F(const Vector3f_PTR p_origin, const Vector3f_PTR p_target, const Vector3f_PTR p_up, Matrix3f_PTR out_rotationMatrix);