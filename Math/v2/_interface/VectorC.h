#pragma once

#include "VectorC_def.h"
#include "v2/_interface/QuaternionC_def.h"
#include <stdbool.h>

extern const VECTOR3F VECTOR3F_ZERO;
extern const VECTOR3F VECTOR3F_RIGHT;
extern const VECTOR3F VECTOR3F_UP;
extern const VECTOR3F VECTOR3F_FORWARD;

bool Vec_Equals_2d(const VECTOR2D_PTR p_left, const VECTOR2D_PTR p_right);
bool Vec_Equals_2f(const VECTOR2F_PTR p_left, const VECTOR2F_PTR p_right);
bool Vec_Equals_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right);

void Vec_Min_2d_2d(const VECTOR2D_PTR p_left, const VECTOR2D_PTR p_right, VECTOR2D_PTR p_out);
void Vec_Min_3f_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out);
void Vec_Min_4f_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out);

void Vec_Add_3f_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out);
void Vec_Add_4f_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out);

void Vec_Mul_2f_1f(const VECTOR2F_PTR p_left, const float p_right, VECTOR2F_PTR p_out);
void Vec_Mul_3f_1f(const VECTOR3F_PTR p_left, const float p_right, VECTOR3F_PTR p_out);
void Vec_Mul_3f_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out);
void Vec_Mul_4f_1f(const VECTOR4F_PTR p_left, const float p_right, VECTOR4F_PTR p_out);
void Vec_Mul_4f_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out);

void Vec_Inv_3f(const VECTOR3F_PTR p_vec, VECTOR3F_PTR p_out);

float Vec_Dot_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right);

void Vec_Cross_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out);

float Vec_Length_3f(const VECTOR3F_PTR p_left);
float Vec_Length_4f(const VECTOR4F_PTR p_left);

void Vec_Normalize_3f(const VECTOR3F_PTR p_vec, VECTOR3F_PTR p_out);

void Vec_Project_3f(const VECTOR3F_PTR p_vec, const VECTOR3F_PTR p_projectedOn, VECTOR3F_PTR p_out);

float Vec_Distance_3f(const VECTOR3F_PTR p_start, const VECTOR3F_PTR p_end);

float Vec_Angle_3f(const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end);
float Vec_Angle_Normalized_3f(const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end);
short int Vec_AngleSign_3f(const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end, VECTOR3F_PTR p_referenceAxis);
void Vec_Rotate_3f(const VECTOR3F_PTR p_vector, const QUATERNION4F_PTR p_rotation, VECTOR3F_PTR p_out);