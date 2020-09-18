#pragma once

#include "VectorC_def.h"
#include "v2/_interface/QuaternionC_def.h"
#include <stdbool.h>

extern const Vector3f Vector3f_ZERO;
extern const Vector3f Vector3f_RIGHT;
extern const Vector3f Vector3f_UP;
extern const Vector3f Vector3f_FORWARD;

bool Vec_Equals_2d(const Vector2d_PTR p_left, const Vector2d_PTR p_right);
bool Vec_Equals_2f(const Vector2f_PTR p_left, const Vector2f_PTR p_right);
bool Vec_Equals_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right);

void Vec_Min_2d_2d(const Vector2d_PTR p_left, const Vector2d_PTR p_right, Vector2d_PTR p_out);
void Vec_Min_3f_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out);
void Vec_Min_4f_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out);

void Vec_Add_2f_2f(const Vector2f_PTR p_left, const Vector2f_PTR p_right, Vector2f_PTR p_out);
void Vec_Add_3f_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out);
void Vec_Add_4f_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out);

void Vec_Mul_2f_1f(const Vector2f_PTR p_left, const float p_right, Vector2f_PTR p_out);
void Vec_Mul_3f_1f(const Vector3f_PTR p_left, const float p_right, Vector3f_PTR p_out);
void Vec_Mul_3c_1f(const Vector3c_PTR p_left, const float p_right, Vector3c_PTR p_out);
void Vec_Mul_3f_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out);
void Vec_Mul_4f_1f(const Vector4f_PTR p_left, const float p_right, Vector4f_PTR p_out);
void Vec_Mul_4f_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out);

void Vec_Inv_3f(const Vector3f_PTR p_vec, Vector3f_PTR p_out);

float Vec_Dot_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right);

void Vec_Cross_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out);

float Vec_Length_3f(const Vector3f_PTR p_left);
float Vec_Length_4f(const Vector4f_PTR p_left);

void Vec_Normalize_3f(const Vector3f_PTR p_vec, Vector3f_PTR p_out);

void Vec_Project_3f(const Vector3f_PTR p_vec, const Vector3f_PTR p_projectedOn, Vector3f_PTR p_out);

float Vec_Distance_3f(const Vector3f_PTR p_start, const Vector3f_PTR p_end);

#if 0
double Vec_SignedAngle_2i(const Vector2i_PTR p_begin, const Vector2i_PTR p_end);
#endif
float Vec_Angle_3f(const Vector3f_PTR p_begin, const Vector3f_PTR p_end);
float Vec_Angle_Normalized_3f(const Vector3f_PTR p_begin, const Vector3f_PTR p_end);
short int Vec_AngleSign_3f(const Vector3f_PTR p_begin, const Vector3f_PTR p_end, Vector3f_PTR p_referenceAxis);
void Vec_Rotate_3f(const Vector3f_PTR p_vector, const Quaternion4f_PTR p_rotation, Vector3f_PTR p_out);