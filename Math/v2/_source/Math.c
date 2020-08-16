#include "VectorC.h"

#include "Constants.h"

#include <math.h>
#include <stdlib.h>

#include "Functional/Equals/Equals.h"

/* VECTOR - Equals */
bool Vec_Equals_2d(const VECTOR2D_PTR p_left, const VECTOR2D_PTR p_right)
{
	return Equals_double(&p_left->x, &p_right->x)
		&& Equals_double(&p_left->y, &p_right->y);
};

bool Vec_Equals_2f(const VECTOR2F_PTR p_left, const VECTOR2F_PTR p_right)
{
	return Equals_float(&p_left->x, &p_right->x)
		&& Equals_float(&p_left->y, &p_right->y);
};

bool Vec_Equals_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right)
{
	return Equals_float(&p_left->x, &p_right->x)
		&& Equals_float(&p_left->y, &p_right->y)
		&& Equals_float(&p_left->z, &p_right->z);
};

/* VECTOR - MIN */

inline void Vec_Min_Xd_Xd(const char* p_left, const char* p_right, const short int p_elementCount, char* p_out)
{
	char* l_leftCursor = p_left;
	char* l_rightCursor = p_right;
	char* l_outCursor = p_out;

	for (short int i = 0; i < p_elementCount; i++)
	{
		*(double*)l_outCursor = *(double*)l_leftCursor - *(double*)l_rightCursor;

		l_leftCursor += sizeof(double);
		l_outCursor += sizeof(double);
		l_rightCursor += sizeof(double);
	}
};

inline void Vec_Min_Xf_Xf(const char* p_left, const char* p_right, const short int p_elementCount, char* p_out)
{
	char* l_leftCursor = p_left;
	char* l_rightCursor = p_right;
	char* l_outCursor = p_out;

	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor - *(float*)l_rightCursor;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
};

void Vec_Min_2d_2d(const VECTOR2D_PTR p_left, const VECTOR2D_PTR p_right, VECTOR2D_PTR p_out)
{
	Vec_Min_Xd_Xd(p_left, p_right, 2, p_out);
};

void Vec_Min_3f_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out)
{
	Vec_Min_Xf_Xf(p_left, p_right, 3, p_out);
}

void Vec_Min_4f_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out)
{
	Vec_Min_Xf_Xf(p_left, p_right, 4, p_out);
};

/* VECTOR - ADDITION */

inline void Vec_Add_Xf_Xf(const char* p_left, const char* p_right, const short int p_elementCount, char* p_out)
{
	char* l_leftCursor = p_left;
	char* l_rightCursor = p_right;
	char* l_outCursor = p_out;

	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor + *(float*)l_rightCursor;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
};

void Vec_Add_3f_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out)
{
	Vec_Add_Xf_Xf(p_left, p_right, 3, p_out);
}

void Vec_Add_4f_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out)
{
	Vec_Add_Xf_Xf(p_left, p_right, 4, p_out);
};

/* VECTOR - MULTIPLICATION */

inline void Vec_Mul_Xf_1f(const char* p_left, const short int p_elementCount, const float p_right, char* p_out)
{
	char* l_leftCursor = p_left;
	char* l_outCursor = p_out;
	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor * p_right;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
	}
};

inline void Vec_Mul_Xf_Xf(const char* p_left, const short int p_elementCount, const char* p_right, char* p_out)
{
	char* l_leftCursor = p_left;
	char* l_rightCursor = p_right;
	char* l_outCursor = p_out;
	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor * *(float*)l_rightCursor;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
};

void Vec_Mul_2f_1f(const VECTOR2F_PTR p_left, const float p_right, VECTOR2F_PTR p_out)
{
	Vec_Mul_Xf_1f(p_left, 2, p_right, p_out);
};

void Vec_Mul_3f_1f(const VECTOR3F_PTR p_left, const float p_right, VECTOR3F_PTR p_out)
{
	Vec_Mul_Xf_1f(p_left, 3, p_right, p_out);
};

void Vec_Mul_3f_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out)
{
	Vec_Mul_Xf_Xf(p_left, 3, p_right, p_out);
};

void Vec_Mul_4f_1f(const VECTOR4F_PTR p_left, const float p_right, VECTOR4F_PTR p_out)
{
	Vec_Mul_Xf_1f(p_left, 4, p_right, p_out);
};

void Vec_Mul_4f_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out)
{
	Vec_Mul_Xf_Xf(p_left, 4, p_right, p_out);
};

void Vec_Inv_3f(const VECTOR3F_PTR p_vec, VECTOR3F_PTR p_out)
{
	char* l_vecCursor = p_vec;
	char* l_outCursor = p_out;
	for (short int i = 0; i < 3; i++)
	{
		*(float*)l_outCursor = 1.0f / *(float*)l_vecCursor;

		l_vecCursor += sizeof(float);
		l_outCursor += sizeof(float);
	}
};

/* VECTOR - Geometric operations */

float Vec_Dot_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right)
{
	float l_return = 0.0f;
	char* l_leftCursor = p_left;
	char* l_rightCursor = p_right;
	for (short int i = 0; i < 3; i++)
	{
		l_return += (*(float*)l_leftCursor * *(float*)l_rightCursor);
		l_leftCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
	return l_return;
};

void Vec_Cross_3f(const VECTOR3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out)
{
	p_out->x = (p_left->y * p_right->z) - (p_left->z * p_right->y);
	p_out->y = (p_left->z * p_right->x) - (p_left->x * p_right->z);
	p_out->z = (p_left->x * p_right->y) - (p_left->y * p_right->x);
};

inline float Vec_Length_Xf(const char* p_vec, const short int p_elementCount)
{
	float l_return = 0.0f;
	char* l_vecCursor = p_vec;
	for (short int i = 0; i < p_elementCount; i++)
	{
		l_return += (*(float*)l_vecCursor * *(float*)l_vecCursor);
		l_vecCursor += sizeof(float);
	}
	return sqrtf(l_return);
};

float Vec_Length_3f(const VECTOR3F_PTR p_left)
{
	return Vec_Length_Xf(p_left, 3);
};

float Vec_Length_4f(const VECTOR4F_PTR p_left)
{
	return Vec_Length_Xf(p_left, 4);
};

inline void Vec_Normalize_Xf(const char* p_vec, const short int p_elementCount, char* p_out)
{
	float l_invertedLength = 1 / Vec_Length_Xf(p_vec, p_elementCount);
	char* l_vecCursor = p_vec;
	char* l_outCursor = p_out;
	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_vecCursor * l_invertedLength;

		l_vecCursor += sizeof(float);
		l_outCursor += sizeof(float);
	}
};

void Vec_Normalize_3f(const VECTOR3F_PTR p_vec, VECTOR3F_PTR p_out)
{
	Vec_Normalize_Xf(p_vec, 3, p_out);
};

void Vec_Project_3f(const VECTOR3F_PTR p_vec, const VECTOR3F_PTR p_projectedOn, VECTOR3F_PTR p_out)
{
	Vec_Mul_3f_1f(p_projectedOn, (Vec_Dot_3f(p_vec, p_projectedOn) / Vec_Length_3f(p_projectedOn)), p_out);
};

float Vec_Distance_3f(const VECTOR3F_PTR p_start, const VECTOR3F_PTR p_end)
{
	VECTOR3F l_vec;
	Vec_Min_3f_3f(p_start, p_end, &l_vec);
	return Vec_Length_3f(&l_vec);
};

float Vec_Angle_3f(const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end)
{
	return acosf(
		Vec_Dot_3f(p_begin, p_end) / (Vec_Length_3f(p_begin), Vec_Length_3f(p_end))
	);
};

float Vec_Angle_Normalized_3f(const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end)
{
	return acosf(Vec_Dot_3f(p_begin, p_end));
};

short int Vec_AngleSign_3f(const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end, VECTOR3F_PTR p_referenceAxis)
{
	VECTOR3F l_cross;
	Vec_Cross_3f(p_begin, p_end, &l_cross);
	float l_dot = Vec_Dot_3f(&l_cross, p_referenceAxis);
	return l_dot >= FLOAT_TOLERANCE ? 1 : -1;
};
