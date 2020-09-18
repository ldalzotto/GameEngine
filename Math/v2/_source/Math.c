#include "v2/Math.h"

#include "Functional/Equals/Equals.h"

#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/BoxC.h"
#include "v2/_interface/BoxC_alg.h"
#include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/ColorC.h"
#include "v2/_interface/PlaneC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/RectC.h"
#include "v2/_interface/WindowSize.h"

#include "Constants.h"

#include <math.h> 
#include <stdlib.h>
#include <string.h>

/* MATH - Functions */

float DEG_TO_RAD = (M_PI / 180.0f);
float RAD_TO_DEG = (180.0f / M_PI);

double Math_max(double left, double right)
{
	if (right > left)
	{
		return right;
	}
	else
	{
		return left;
	}
};

double Math_min(double left, double right)
{
	if (right < left)
	{
		return right;
	}
	else
	{
		return left;
	}
};

float Math_clamp01f(float p_value)
{
	if (p_value <= FLOAT_TOLERANCE) { return 0.0f; }
	else if (p_value >= 1.0f + FLOAT_TOLERANCE) { return 1.0f; }
	else { return p_value; };
};

/* VECTOR - Equals */
bool Vec_Equals_2d(const Vector2d_PTR p_left, const Vector2d_PTR p_right)
{
	return Equals_double(&p_left->x, &p_right->x)
		&& Equals_double(&p_left->y, &p_right->y);
};

bool Vec_Equals_2f(const Vector2f_PTR p_left, const Vector2f_PTR p_right)
{
	return Equals_float(&p_left->x, &p_right->x)
		&& Equals_float(&p_left->y, &p_right->y);
};

bool Vec_Equals_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right)
{
	return Equals_float(&p_left->x, &p_right->x)
		&& Equals_float(&p_left->y, &p_right->y)
		&& Equals_float(&p_left->z, &p_right->z);
};

bool Vec_Equals_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right)
{
	return Equals_float(&p_left->x, &p_right->x)
		&& Equals_float(&p_left->y, &p_right->y)
		&& Equals_float(&p_left->z, &p_right->z)
		&& Equals_float(&p_left->w, &p_right->w);
};


const Vector3f Vector3f_ZERO = { 0.0f, 0.0f, 0.0f };
const Vector3f Vector3f_RIGHT = { 1.0f, 0.0f, 0.0f };
const Vector3f Vector3f_UP = { 0.0f, 1.0f, 0.0f };
const Vector3f Vector3f_FORWARD = { 0.0f, 0.0f, 1.0f };

/* VECTOR - MIN */

inline void Vec_Min_Xd_Xd(const char* p_left, const char* p_right, const short int p_elementCount, char* p_out)
{
	char* l_leftCursor = (char*)p_left;
	char* l_rightCursor = (char*)p_right;
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
	char* l_leftCursor = (char*)p_left;
	char* l_rightCursor = (char*)p_right;
	char* l_outCursor = p_out;

	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor - *(float*)l_rightCursor;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
};

void Vec_Min_2d_2d(const Vector2d_PTR p_left, const Vector2d_PTR p_right, Vector2d_PTR p_out)
{
	Vec_Min_Xd_Xd((const char*)p_left, (const char*)p_right, 2, (char*)p_out);
};

void Vec_Min_3f_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out)
{
	Vec_Min_Xf_Xf((const char*)p_left, (const char*)p_right, 3, (char*)p_out);
}

void Vec_Min_4f_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out)
{
	p_out->x = p_left->x - p_right->x;
	p_out->y = p_left->y - p_right->y;
	p_out->z = p_left->z - p_right->z;
	p_out->w = p_left->w - p_right->w;
};

/* VECTOR - ADDITION */

inline void Vec_Add_Xf_Xf(const char* p_left, const char* p_right, const short int p_elementCount, char* p_out)
{
	char* l_leftCursor = (char*)p_left;
	char* l_rightCursor = (char*)p_right;
	char* l_outCursor = (char*)p_out;

	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor + *(float*)l_rightCursor;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
};

void Vec_Add_2f_2f(const Vector2f_PTR p_left, const Vector2f_PTR p_right, Vector2f_PTR p_out)
{
	Vec_Add_Xf_Xf((const char*)p_left, (const char*)p_right, 2, (char*)p_out);
};

void Vec_Add_3f_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out)
{
	Vec_Add_Xf_Xf((const char*)p_left, (const char*)p_right, 3, (char*)p_out);
}

void Vec_Add_4f_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out)
{
	Vec_Add_Xf_Xf((const char*)p_left, (const char*)p_right, 4, (char*)p_out);
};

/* VECTOR - MULTIPLICATION */

inline void Vec_Mul_Xf_1f(const char* p_left, const short int p_elementCount, const float p_right, char* p_out)
{
	char* l_leftCursor = (char*)p_left;
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
	char* l_leftCursor = (char*)p_left;
	char* l_rightCursor = (char*)p_right;
	char* l_outCursor = p_out;
	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_leftCursor * *(float*)l_rightCursor;

		l_leftCursor += sizeof(float);
		l_outCursor += sizeof(float);
		l_rightCursor += sizeof(float);
	}
};

void Vec_Mul_2f_1f(const Vector2f_PTR p_left, const float p_right, Vector2f_PTR p_out)
{
	Vec_Mul_Xf_1f((const char*)p_left, 2, p_right, (char*)p_out);
};

void Vec_Mul_3f_1f(const Vector3f_PTR p_left, const float p_right, Vector3f_PTR p_out)
{
	Vec_Mul_Xf_1f((const char*)p_left, 3, p_right, (char*)p_out);
};

void Vec_Mul_3c_1f(const Vector3c_PTR p_left, const float p_right, Vector3c_PTR p_out)
{
	p_out->x = (char)((float)p_left->x * p_right);
	p_out->y = (char)((float)p_left->y * p_right);
	p_out->z = (char)((float)p_left->z * p_right);
};

void Vec_Mul_3f_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out)
{
	Vec_Mul_Xf_Xf((const char*)p_left, 3, (const char*)p_right, (char*)p_out);
};

void Vec_Mul_4f_1f(const Vector4f_PTR p_left, const float p_right, Vector4f_PTR p_out)
{
	Vec_Mul_Xf_1f((const char*)p_left, 4, p_right, (char*)p_out);
};

void Vec_Mul_4f_4f(const Vector4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out)
{
	Vec_Mul_Xf_Xf((const char*)p_left, 4, (const char*)p_right, (char*)p_out);
};

void Vec_Inv_3f(const Vector3f_PTR p_vec, Vector3f_PTR p_out)
{
	char* l_vecCursor = (char*)p_vec;
	char* l_outCursor = (char*)p_out;
	for (short int i = 0; i < 3; i++)
	{
		*(float*)l_outCursor = 1.0f / *(float*)l_vecCursor;

		l_vecCursor += sizeof(float);
		l_outCursor += sizeof(float);
	}
};

/* VECTOR - Geometric operations */

float Vec_Dot_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right)
{
	return (p_left->Points[0] * p_right->Points[0]) + (p_left->Points[1] * p_right->Points[1]) + (p_left->Points[2] * p_right->Points[2]);
};

void Vec_Cross_3f(const Vector3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out)
{
	p_out->x = (p_left->y * p_right->z) - (p_left->z * p_right->y);
	p_out->y = (p_left->z * p_right->x) - (p_left->x * p_right->z);
	p_out->z = (p_left->x * p_right->y) - (p_left->y * p_right->x);
};

inline float Vec_Length_Xf(const char* p_vec, const short int p_elementCount)
{
	float l_return = 0.0f;
	char* l_vecCursor = (char*)p_vec;
	for (short int i = 0; i < p_elementCount; i++)
	{
		l_return += (*(float*)l_vecCursor * *(float*)l_vecCursor);
		l_vecCursor += sizeof(float);
	}
	return sqrtf(l_return);
};

float Vec_Length_3f(const Vector3f_PTR p_left)
{
	return Vec_Length_Xf((const char*)p_left, 3);
};

float Vec_Length_4f(const Vector4f_PTR p_left)
{
	return Vec_Length_Xf((const char*)p_left, 4);
};

inline void Vec_Normalize_Xf(const char* p_vec, const short int p_elementCount, char* p_out)
{
	float l_invertedLength = 1 / Vec_Length_Xf(p_vec, p_elementCount);
	char* l_vecCursor = (char*)p_vec;
	char* l_outCursor = p_out;
	for (short int i = 0; i < p_elementCount; i++)
	{
		*(float*)l_outCursor = *(float*)l_vecCursor * l_invertedLength;

		l_vecCursor += sizeof(float);
		l_outCursor += sizeof(float);
	}
};

void Vec_Normalize_3f(const Vector3f_PTR p_vec, Vector3f_PTR p_out)
{
	Vec_Normalize_Xf((const char*)p_vec, 3, (char*)p_out);
};

void Vec_Project_3f(const Vector3f_PTR p_vec, const Vector3f_PTR p_projectedOn, Vector3f_PTR p_out)
{
	Vec_Mul_3f_1f(p_projectedOn, (Vec_Dot_3f(p_vec, p_projectedOn) / Vec_Length_3f(p_projectedOn)), p_out);
};

float Vec_Distance_3f(const Vector3f_PTR p_start, const Vector3f_PTR p_end)
{
	Vector3f l_vec;
	Vec_Min_3f_3f(p_start, p_end, &l_vec);
	return Vec_Length_3f(&l_vec);
};

#if 0
double Vec_SignedAngle_2i(const Vector2i_PTR p_begin, const Vector2i_PTR p_end)
{
	return atan2((double)p_end->y, (double)p_end->x) - atan2((double)p_begin->y, (double)p_begin->x);
	// return atan2(((double)p_begin->x * p_end->y) - ((double)p_begin->y * p_end->x), ((double)p_begin->x * p_end->x) - ((double)p_begin->y * p_end->y));
};
#endif


float Vec_Angle_3f(const Vector3f_PTR p_begin, const Vector3f_PTR p_end)
{
	return acosf(
		Vec_Dot_3f(p_begin, p_end) / (Vec_Length_3f(p_begin), Vec_Length_3f(p_end))
	);
};

float Vec_Angle_Normalized_3f(const Vector3f_PTR p_begin, const Vector3f_PTR p_end)
{
	return acosf(Vec_Dot_3f(p_begin, p_end));
};

short int Vec_AngleSign_3f(const Vector3f_PTR p_begin, const Vector3f_PTR p_end, Vector3f_PTR p_referenceAxis)
{
	Vector3f l_cross;
	Vec_Cross_3f(p_begin, p_end, &l_cross);
	float l_dot = Vec_Dot_3f(&l_cross, p_referenceAxis);
	return l_dot >= FLOAT_TOLERANCE ? 1 : -1;
};

void Vec_Rotate_3f(const Vector3f_PTR p_vector, const Quaternion4f_PTR p_rotation, Vector3f_PTR p_out)
{
	Quaternion4f l_vectorAsQuat;
	Quat_Build_3f_1f(p_vector, 0.0f, &l_vectorAsQuat);

	Quaternion4f l_rotatedVector;
	Quat_Mul(p_rotation, &l_vectorAsQuat, &l_rotatedVector);
	{
		Quaternion4f l_tmp;
		Quat_conjugate(p_rotation, &l_tmp);

		Quaternion4f l_rotatedVector_cpy = l_rotatedVector;
		Quat_Mul(&l_rotatedVector_cpy, &l_tmp, &l_rotatedVector);
	}

	Vec_Normalize_3f(&l_rotatedVector.Vec, p_out);
};


/******************** QUATERNION ************************/

const Quaternion4f Quaternion4f_IDENTITY = { 0.0f, 0.0f, 0.0f, 1.0f };

bool Quat_Equals(const Quaternion4f_PTR p_left, const Quaternion4f_PTR p_right)
{
	return Vec_Equals_4f(&p_left->Points, &p_right->Points);
};

void Quat_Build_4f(const Vector4f_PTR p_points, Quaternion4f_PTR p_out)
{
	p_out->Points = *p_points;
};

void Quat_Build_3f_1f(const Vector3f_PTR p_vec, float p_scal, Quaternion4f_PTR p_out)
{
	p_out->Vec = *p_vec;
	p_out->Scal = p_scal;
};

/* QUATERNION - OPERATION */

void Quat_Normalize(const Quaternion4f_PTR p_quat, Quaternion4f_PTR p_out)
{
	Vec_Mul_4f_1f(&p_quat->Points, 1.0f / Vec_Length_4f(&p_quat->Points), (Vector4f_PTR)p_out);
};

void Quat_Mul(const Quaternion4f_PTR p_left, const Quaternion4f_PTR p_right, Quaternion4f_PTR p_out)
{
	Vec_Mul_3f_1f(&p_left->Vec, p_right->w, &p_out->Vec);
	{
		Vector3f l_tmp;
		Vec_Mul_3f_1f(&p_right->Vec, p_left->w, &l_tmp);
		Vec_Add_3f_3f(&p_out->Vec, &l_tmp, &p_out->Vec);

		Vec_Cross_3f(&p_left->Vec, &p_right->Vec, &l_tmp);
		Vec_Add_3f_3f(&p_out->Vec, &l_tmp, &p_out->Vec);
	}

	p_out->w = (p_left->w * p_right->w) - Vec_Dot_3f(&p_left->Vec, &p_right->Vec);
	Quat_Normalize(p_out, p_out);
};

void Quat_Cross(const Quaternion4f_PTR p_left, const  Quaternion4f_PTR p_right, Quaternion4f_PTR p_out)
{
	Vector3f l_rotatedLeft;
	Vec_Rotate_3f((const Vector3f_PTR)&Vector3f_FORWARD, p_left, &l_rotatedLeft);

	Vector3f l_rotatedRight;
	Vec_Rotate_3f((const Vector3f_PTR)&Vector3f_FORWARD, p_right, &l_rotatedRight);

	Vector3f l_crossResult;
	Vec_Cross_3f(&l_rotatedLeft, &l_rotatedRight, &l_crossResult);
	Quat_RotateAround(&l_crossResult, 0.0f, p_out);
};

void Quat_ExtractAxis(const Quaternion4f_PTR quat, Matrix3f_PTR out_axis)
{
	float l_qxx = quat->x * quat->x;
	float l_qxy = quat->x * quat->y;
	float l_qxz = quat->x * quat->z;
	float l_qxw = quat->x * quat->w;

	float l_qyy = quat->y * quat->y;
	float l_qyz = quat->y * quat->z;
	float l_qyw = quat->y * quat->w;

	float l_qzz = quat->z * quat->z;
	float l_qzw = quat->z * quat->w;

	//RIGHT
	out_axis->_00 = 1 - (2 * l_qyy) - (2 * l_qzz);
	out_axis->_01 = (2 * l_qxy) + (2 * l_qzw);
	out_axis->_02 = (2 * l_qxz) - (2 * l_qyw);

	//UP	
	out_axis->_10 = (2 * l_qxy) - (2 * l_qzw);
	out_axis->_11 = 1 - (2 * l_qxx) - (2 * l_qzz);
	out_axis->_12 = (2 * l_qyz) + (2 * l_qxw);

	//Forwar
	out_axis->_20 = (2 * l_qxz) + (2 * l_qyw);
	out_axis->_21 = (2 * l_qyz) - (2 * l_qxw);
	out_axis->_22 = 1 - (2 * l_qxx) - (2 * l_qyy);

	Vec_Normalize_3f(&out_axis->Col0, &out_axis->Col0);
	Vec_Normalize_3f(&out_axis->Col1, &out_axis->Col1);
	Vec_Normalize_3f(&out_axis->Col2, &out_axis->Col2);
};


/* QUATERNIOn - BUILD */

void Quat_FromDirection(const Vector3f_PTR p_vec, Quaternion4f_PTR p_out)
{
	float l_angle = Vec_Angle_3f((const Vector3f_PTR)&Vector3f_FORWARD, p_vec);
	Vec_Mul_3f_1f(p_vec, sinf(l_angle * 0.5f), &p_out->Vec);
	p_out->w = cosf(l_angle * 0.5f);
};

void Quat_FromAxis(const Matrix3f_PTR p_axis, Quaternion4f_PTR p_out)
{
	const Vector3f_PTR l_right = &p_axis->Right;
	const Vector3f_PTR l_up = &p_axis->Up;
	const Vector3f_PTR l_forward = &p_axis->Forward;


	// We calculate the four square roots and get the higher one.
	float qxDiag = fmaxf(1 + l_right->x - l_up->y - l_forward->z, 0.0f);
	float qyDiag = fmaxf(1 + l_up->y - l_right->x - l_forward->z, 0.0f);
	float qzDiag = fmaxf(1 + l_forward->z - l_right->x - l_up->y, 0.0f);
	float qwDiag = fmaxf(1 + l_right->x + l_up->y + l_forward->z, 0.0f);

	int l_diagonalIndex = 0;
	float l_biggestDiagonalValue = qxDiag;
	if (qyDiag > l_biggestDiagonalValue)
	{
		l_biggestDiagonalValue = qyDiag;
		l_diagonalIndex = 1;
	}
	if (qzDiag > l_biggestDiagonalValue)
	{
		l_biggestDiagonalValue = qzDiag;
		l_diagonalIndex = 2;
	}
	if (qwDiag > l_biggestDiagonalValue)
	{
		l_biggestDiagonalValue = qwDiag;
		l_diagonalIndex = 3;
	}

	l_biggestDiagonalValue = 0.5f * sqrtf(l_biggestDiagonalValue);
	float mult = 1 / (4.0f * l_biggestDiagonalValue);

	switch (l_diagonalIndex)
	{
	case 0:
	{
		p_out->x = l_biggestDiagonalValue;
		p_out->y = (l_right->y + l_up->x) * mult;
		p_out->z = (l_forward->x + l_right->z) * mult;
		p_out->w = (l_up->z - l_forward->y) * mult;
	}
	break;
	case 1:
	{
		p_out->x = (l_right->y + l_up->x) * mult;
		p_out->y = l_biggestDiagonalValue;
		p_out->z = (l_up->z + l_forward->y) * mult;
		p_out->w = (l_forward->x - l_right->z) * mult;
	}
	break;
	case 2:
	{
		p_out->x = (l_forward->x + l_right->z) * mult;
		p_out->y = (l_up->z + l_forward->y) * mult;
		p_out->z = l_biggestDiagonalValue;
		p_out->w = (l_right->y - l_up->x) * mult;
	}
	break;
	case 3:
	{
		p_out->x = (l_up->z - l_forward->y) * mult;
		p_out->y = (l_forward->x - l_right->z) * mult;
		p_out->z = (l_right->y - l_up->x) * mult;
		p_out->w = l_biggestDiagonalValue;
	}
	break;
	}

};

void Quat_FromEulerAngle(const Vector3f_PTR p_eulerAngle, Quaternion4f_PTR p_out)
{
	{
		Quaternion4f l_yaw;
		Quat_RotateAround((const Vector3f_PTR)&Vector3f_UP, p_eulerAngle->y, &l_yaw);
		p_out->x = 0.0f; p_out->y = 0.0f; p_out->z = 0.0f; p_out->w = 1.0f;
		Quaternion4f l_tmp;
		memcpy(&l_tmp, p_out, sizeof(Quaternion4f));
		Quat_Mul(&l_tmp, &l_yaw, p_out);
	}
	{
		Quaternion4f l_pitch;
		Quat_RotateAround((const Vector3f_PTR)&Vector3f_RIGHT, p_eulerAngle->x, &l_pitch);
		Quaternion4f l_tmp;
		memcpy(&l_tmp, p_out, sizeof(Quaternion4f));
		Quat_Mul(&l_tmp, &l_pitch, p_out);
	}
	{
		Quaternion4f l_roll;
		Quat_RotateAround((const Vector3f_PTR)&Vector3f_FORWARD, p_eulerAngle->z, &l_roll);
		Quaternion4f l_tmp;
		memcpy(&l_tmp, p_out, sizeof(Quaternion4f));
		Quat_Mul(&l_tmp, &l_roll, p_out);
	}

};

void Quat_FromTo(const Vector3f_PTR p_from, const  Vector3f_PTR p_to, Quaternion4f_PTR p_out)
{
	float l_angle = Vec_Angle_3f(p_from, p_to);
	Vector3f l_rotationAxis;
	Vec_Cross_3f(p_from, p_to, &l_rotationAxis);
	Quat_RotateAround(&l_rotationAxis, l_angle, p_out);
};

void Quat_conjugate(const Quaternion4f_PTR p_quat, Quaternion4f_PTR p_out)
{
	Vec_Mul_3f_1f(&p_quat->Vec, -1.0f, &p_out->Vec);
	p_out->Scal = p_quat->Scal;
};


void Quat_RotateAround(const Vector3f_PTR p_axis, const  float p_angle, Quaternion4f_PTR out_quat)
{
	Vec_Mul_3f_1f(p_axis, sinf(p_angle * 0.5f), &out_quat->Vec);
	out_quat->w = cosf(p_angle * 0.5f);
};


/************************ MATRIX *************************/

#if 1

extern const Matrix4f Matrix4f_IDENTITYF = { 1.0f, 0.0f, 0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

inline float Mat_Mul_line_column_Xf(const char* p_left_line, const char* p_right_column, const short int p_leftColumnCount, const size_t p_fullColumnSize)
{
	float l_return = 0.0f;
	char* l_left_lineCursor = (char*)p_left_line;
	char* l_right_columnCursor = (char*)p_right_column;

	for (short int i = 0; i < p_leftColumnCount; i++)
	{
		l_return += (*((float*)l_left_lineCursor) * *((float*)l_right_columnCursor));

		l_left_lineCursor += p_fullColumnSize;
		l_right_columnCursor += sizeof(float);
	}

	return l_return;
};

inline void Mat_Mul_MXxXf_MXxXf(const char* p_left, const char* p_right,
	const short int p_left_lineCount, const short int p_right_columnCount,
	const size_t p_left_lineSize, const size_t p_right_columnSize,
	char* p_out)
{
	char* l_leftCursor = (char*)p_left;
	char* l_rightCursor = (char*)p_right;
	char* l_outCursor = (char*)p_out;

	for (short int i = 0; i < p_right_columnCount; i++)
	{
		for (short int j = 0; j < p_left_lineCount; j++)
		{
			*(float*)l_outCursor = Mat_Mul_line_column_Xf(l_leftCursor, l_rightCursor, p_left_lineCount, p_right_columnSize);

			l_outCursor += sizeof(float);
			l_leftCursor += sizeof(float);
		}

		l_leftCursor = (char*)p_left;
		l_rightCursor += p_right_columnSize;
	}
};

inline void Mat_Mul_MXxXf_f(const float* p_mat, const float p_value, float* p_out, short int p_matrix_elementCount)
{
	char* l_mat_cursor = (char*)p_mat;

	for (short int i = 0; i < p_matrix_elementCount; i++)
	{
		*(float*)l_mat_cursor *= p_value;
		l_mat_cursor += sizeof(float);
	}
};

inline float Mat_Det_M4F(const Matrix4f_PTR p_mat, const short int p_colIndex, const short int p_lineIndex)
{
	Matrix3f l_mat3x3 = { 0 };
	{

		char* l_mat3_cursor = (char*)l_mat3x3.Points;
		char* l_mat_cursor = (char*)p_mat->Points;

		for (short int c = 0; c < 4; c++)
		{
			if (c != p_colIndex)
			{

				for (short int l = 0; l < 4; l++)
				{
					if (l != p_lineIndex)
					{
						*(float*)l_mat3_cursor = *(float*)l_mat_cursor;
						l_mat3_cursor += sizeof(float);
					}

					l_mat_cursor += sizeof(float);
				}
			}
			else
			{
				l_mat_cursor += sizeof(Vector4f);
			}
		}
	}

	return
		(l_mat3x3._00 * ((l_mat3x3._11 * l_mat3x3._22) - (l_mat3x3._12 * l_mat3x3._21))) +
		(l_mat3x3._10 * ((l_mat3x3._21 * l_mat3x3._02) - (l_mat3x3._22 * l_mat3x3._01))) +
		(l_mat3x3._20 * ((l_mat3x3._01 * l_mat3x3._12) - (l_mat3x3._02 * l_mat3x3._11)));
};


void Mat_Mul_M4F_M4F(const Matrix4f_PTR p_left, const Matrix4f_PTR p_right, Matrix4f_PTR p_out)
{
	p_out->_00 = (p_left->_00 * p_right->_00) + (p_left->_10 * p_right->_01) + (p_left->_20 * p_right->_02) + (p_left->_30 * p_right->_03);
	p_out->_01 = (p_left->_01 * p_right->_00) + (p_left->_11 * p_right->_01) + (p_left->_21 * p_right->_02) + (p_left->_31 * p_right->_03);
	p_out->_02 = (p_left->_02 * p_right->_00) + (p_left->_12 * p_right->_01) + (p_left->_22 * p_right->_02) + (p_left->_32 * p_right->_03);
	p_out->_03 = (p_left->_03 * p_right->_00) + (p_left->_13 * p_right->_01) + (p_left->_23 * p_right->_02) + (p_left->_33 * p_right->_03);

	p_out->_10 = (p_left->_00 * p_right->_10) + (p_left->_10 * p_right->_11) + (p_left->_20 * p_right->_12) + (p_left->_30 * p_right->_13);
	p_out->_11 = (p_left->_01 * p_right->_10) + (p_left->_11 * p_right->_11) + (p_left->_21 * p_right->_12) + (p_left->_31 * p_right->_13);
	p_out->_12 = (p_left->_02 * p_right->_10) + (p_left->_12 * p_right->_11) + (p_left->_22 * p_right->_12) + (p_left->_32 * p_right->_13);
	p_out->_13 = (p_left->_03 * p_right->_10) + (p_left->_13 * p_right->_11) + (p_left->_23 * p_right->_12) + (p_left->_33 * p_right->_13);

	p_out->_20 = (p_left->_00 * p_right->_20) + (p_left->_10 * p_right->_21) + (p_left->_20 * p_right->_22) + (p_left->_30 * p_right->_23);
	p_out->_21 = (p_left->_01 * p_right->_20) + (p_left->_11 * p_right->_21) + (p_left->_21 * p_right->_22) + (p_left->_31 * p_right->_23);
	p_out->_22 = (p_left->_02 * p_right->_20) + (p_left->_12 * p_right->_21) + (p_left->_22 * p_right->_22) + (p_left->_32 * p_right->_23);
	p_out->_23 = (p_left->_03 * p_right->_20) + (p_left->_13 * p_right->_21) + (p_left->_23 * p_right->_22) + (p_left->_33 * p_right->_23);

	p_out->_30 = (p_left->_00 * p_right->_30) + (p_left->_10 * p_right->_31) + (p_left->_20 * p_right->_32) + (p_left->_30 * p_right->_33);
	p_out->_31 = (p_left->_01 * p_right->_30) + (p_left->_11 * p_right->_31) + (p_left->_21 * p_right->_32) + (p_left->_31 * p_right->_33);
	p_out->_32 = (p_left->_02 * p_right->_30) + (p_left->_12 * p_right->_31) + (p_left->_22 * p_right->_32) + (p_left->_32 * p_right->_33);
	p_out->_33 = (p_left->_03 * p_right->_30) + (p_left->_13 * p_right->_31) + (p_left->_23 * p_right->_32) + (p_left->_33 * p_right->_33);
};

void Mat_Mul_M4F_V4F(const Matrix4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out)
{
	Mat_Mul_MXxXf_MXxXf((const char*)p_left, (const char*)p_right, 4, 1, sizeof(Vector4f), sizeof(Vector4f), (char*)p_out);
};

void Mat_Mul_M4F_V4F_Homogeneous(const Matrix4f_PTR p_projectionmatrix, const Vector4f_PTR p_pos, Vector4f_PTR out_pos)
{
	Mat_Mul_M4F_V4F(p_projectionmatrix, p_pos, out_pos);
	Vec_Mul_4f_1f(out_pos, 1.0f / out_pos->w, out_pos);
};

void Mat_Mul_M3F_M3F(const Matrix3f_PTR p_left, const Matrix3f_PTR p_right, Matrix3f_PTR p_out)
{
	Mat_Mul_MXxXf_MXxXf((const char*)p_left, (const char*)p_right, 3, 3, sizeof(Vector4f), sizeof(Vector4f), (char*)p_out);
};

void Mat_Mul_M3F_V3F(const Matrix3f_PTR p_left, const Vector3f_PTR p_right, Vector3f_PTR p_out)
{
	Mat_Mul_MXxXf_MXxXf((const char*)p_left, (const char*)p_right, 3, 1, sizeof(Vector3f), sizeof(Vector3f), (char*)p_out);
};

void Mat_Mul_M3F_1F(const Matrix3f_PTR p_left, const float p_right, Matrix3f_PTR p_out)
{
	Mat_Mul_MXxXf_f((const float*)p_left, p_right, (float*)p_out, 9);
};

void Mat_Inv_M4F(const Matrix4f_PTR p_matrix, Matrix4f_PTR p_out)
{
	float l_det = (p_matrix->_00 * Mat_Det_M4F(p_matrix, 0, 0)) - (p_matrix->_01 * Mat_Det_M4F(p_matrix, 0, 1)) + (p_matrix->_02 * Mat_Det_M4F(p_matrix, 0, 2)) - (p_matrix->_03 * Mat_Det_M4F(p_matrix, 0, 3));

	{
		p_out->_00 = Mat_Det_M4F(p_matrix, 0, 0);
		p_out->_01 = -Mat_Det_M4F(p_matrix, 1, 0);
		p_out->_02 = Mat_Det_M4F(p_matrix, 2, 0);
		p_out->_03 = -Mat_Det_M4F(p_matrix, 3, 0);
		p_out->_10 = -Mat_Det_M4F(p_matrix, 0, 1);
		p_out->_11 = Mat_Det_M4F(p_matrix, 1, 1);
		p_out->_12 = -Mat_Det_M4F(p_matrix, 2, 1);
		p_out->_13 = Mat_Det_M4F(p_matrix, 3, 1);
		p_out->_20 = Mat_Det_M4F(p_matrix, 0, 2);
		p_out->_21 = -Mat_Det_M4F(p_matrix, 1, 2);
		p_out->_22 = Mat_Det_M4F(p_matrix, 2, 2);
		p_out->_23 = -Mat_Det_M4F(p_matrix, 3, 2);
		p_out->_30 = -Mat_Det_M4F(p_matrix, 0, 3);
		p_out->_31 = Mat_Det_M4F(p_matrix, 1, 3);
		p_out->_32 = -Mat_Det_M4F(p_matrix, 2, 3);
		p_out->_33 = Mat_Det_M4F(p_matrix, 3, 3);
	}

	Mat_Mul_MXxXf_f(p_out->Points, 1.0f / l_det, p_out->Points, 16);
};

/* MATRIX - TRS related operaions */

void Mat_Translation_M4F(const Vector3f_PTR p_translation, Matrix4f_PTR out_mat)
{
	out_mat->Col3.Vec3 = *p_translation;
};

void Mat_RotationAxis_M4F(const Matrix3f_PTR p_axis, Matrix4f_PTR out_mat)
{
	out_mat->Col0.Vec3 = p_axis->Col0;
	out_mat->Col1.Vec3 = p_axis->Col1;
	out_mat->Col2.Vec3 = p_axis->Col2;
};

void Mat_RotationSeparatedAxis_M4F(const Vector3f_PTR p_right, const Vector3f_PTR p_up, const Vector3f_PTR p_forward, Matrix4f_PTR out_mat)
{
	out_mat->Col0.Vec3 = *p_right;
	out_mat->Col1.Vec3 = *p_up;
	out_mat->Col2.Vec3 = *p_forward;
};

void Mat_Scale_M4F(const Vector3f_PTR p_scale, Matrix4f_PTR out_mat)
{
	Vec_Mul_3f_1f(&out_mat->Col0.Vec3, p_scale->x, &out_mat->Col0.Vec3);
	Vec_Mul_3f_1f(&out_mat->Col1.Vec3, p_scale->y, &out_mat->Col1.Vec3);
	Vec_Mul_3f_1f(&out_mat->Col2.Vec3, p_scale->z, &out_mat->Col2.Vec3);
};

void Mat_TRS_Axis_M4F(const Vector3f_PTR p_position, const Matrix3f_PTR p_axis, const Vector3f_PTR p_scale, Matrix4f_PTR out_TRS)
{
	*out_TRS = Matrix4f_IDENTITYF;
	Mat_Translation_M4F(p_position, out_TRS);
	Mat_RotationAxis_M4F(p_axis, out_TRS);
	Mat_Scale_M4F(p_scale, out_TRS);
};

void Mat_TRS_Quat_M4F(const Vector3f_PTR p_position, const Quaternion4f_PTR p_quat, const Vector3f_PTR p_scale, Matrix4f_PTR out_TRS)
{
	Matrix3f l_axis; Quat_ExtractAxis(p_quat, &l_axis);
	Mat_TRS_Axis_M4F(p_position, &l_axis, p_scale, out_TRS);
};

void Mat_GetTranslation_M4F(const Matrix4f_PTR p_mat, Vector4f_PTR out_translation)
{
	*out_translation = p_mat->Col3;
};

void Mat_GetScale_M4F(const Matrix4f_PTR p_trs, Vector4f_PTR out_scale)
{
	out_scale->x = Vec_Length_3f(&p_trs->Col0.Vec3);
	out_scale->y = Vec_Length_3f(&p_trs->Col1.Vec3);
	out_scale->z = Vec_Length_3f(&p_trs->Col2.Vec3);
	out_scale->w = 0.0f;
};

/* MATRIX - Rotation related operations */

void Mat_Perspective_M4F(const float p_fov, const float p_aspect, const float p_near, const float p_far, Matrix4f_PTR p_out)
{
	float l_halfTan = tanf(p_fov / 2.0f);


#if 1
	p_out->_00 = 1.0f / (p_aspect * l_halfTan);
	p_out->_01 = 0.0f;
	p_out->_02 = 0.0f;
	p_out->_03 = 0.0f;

	p_out->_10 = 0.0f;
	p_out->_11 = 1.0f / l_halfTan;
	p_out->_12 = 0.0f;
	p_out->_13 = 0.0f;

	p_out->_20 = 0.0f;
	p_out->_21 = 0.0f;
	p_out->_22 = (p_far + p_near) / (p_far - p_near);
	p_out->_23 = 1.0f;

	p_out->_30 = 0.0f;
	p_out->_31 = 0.0f;
	p_out->_32 = (-2.0f * p_far * p_near) / (p_far - p_near);
	p_out->_33 = 0.0f;
#endif

#if 0
	p_out->_00 = 1.0f / (p_aspect * l_halfTan);
	p_out->_01 = 0.0f;
	p_out->_02 = 0.0f;
	p_out->_03 = 0.0f;

	p_out->_10 = 0.0f;
	p_out->_11 = 1.0f / l_halfTan;
	p_out->_12 = 0.0f;
	p_out->_13 = 0.0f;

	p_out->_20 = 0.0f;
	p_out->_21 = 0.0f;
	p_out->_22 = (p_far + p_near) / (p_far - p_near);
	p_out->_23 = 1.0f;

	p_out->_30 = 0.0f;
	p_out->_31 = 0.0f;
	p_out->_32 = (-2.0f * p_far * p_near) / (p_far - p_near);
	p_out->_33 = 0.0f;
#endif
}

void Mat_ViewMatrix_M4F(const Vector3f_PTR p_worldPosition, const Vector3f_PTR p_forward, const Vector3f_PTR p_up, Matrix4f_PTR out_viewMatrix)
{
	Vector3f tmp_vec3_0; Matrix4f tmp_mat4_0; Matrix3f tmp_mat3_0;
	Vector3f l_target = *p_forward;

	Vec_Add_3f_3f(p_worldPosition, &l_target, &l_target);

	Vector3f l_up = *p_up;
	Vec_Mul_3f_1f(&l_up, -1.0f, &l_up);

	tmp_vec3_0 = (Vector3f){ 1.0f, 1.0f, 1.0f };
	Mat_LookAtRotation_ViewMatrix_F(p_worldPosition, &l_target, &l_up, &tmp_mat3_0);
	Mat_TRS_Axis_M4F(p_worldPosition, &tmp_mat3_0, &tmp_vec3_0, &tmp_mat4_0);

	Mat_Inv_M4F(&tmp_mat4_0, out_viewMatrix);
};

void Mat_LookAtRotation_ViewMatrix_F(const Vector3f_PTR p_origin, const Vector3f_PTR p_target, const Vector3f_PTR p_up, Matrix3f_PTR out_rotationMatrix)
{
	Vector3f l_forward;
	{
		Vec_Min_3f_3f(p_target, p_origin, &l_forward);
		Vec_Normalize_3f(&l_forward, &l_forward);
	}
	Vector3f l_right;
	{
		Vec_Cross_3f(&l_forward, p_up, &l_right);
		Vec_Normalize_3f(&l_right, &l_right);

		//WARNING : this is true only for view matrices (camera).
		Vec_Mul_3f_1f(&l_right, -1.0f, &l_right);
	}
	Vector3f l_up;
	{
		Vec_Cross_3f(&l_right, &l_forward, &l_up);
		Vec_Normalize_3f(&l_up, &l_up);

		//WARNING : this is true only for view matrices (camera).
		Vec_Mul_3f_1f(&l_up, -1.0f, &l_up);
	}

	out_rotationMatrix->Col0 = l_right;
	out_rotationMatrix->Col1 = l_up;
	out_rotationMatrix->Col2 = l_forward;
};

void Mat_LookAtRotation_F(const Vector3f_PTR p_origin, const Vector3f_PTR p_target, const Vector3f_PTR p_up, Matrix3f_PTR out_rotationMatrix)
{
	Vector3f l_forward;
	{
		Vec_Min_3f_3f(p_target, p_origin, &l_forward);
		Vec_Normalize_3f(&l_forward, &l_forward);
	}
	Vector3f l_right;
	{
		Vec_Cross_3f(&l_forward, p_up, &l_right);
		Vec_Normalize_3f(&l_right, &l_right);
	}
	Vector3f l_up;
	{
		Vec_Cross_3f(&l_right, &l_forward, &l_up);
		Vec_Normalize_3f(&l_up, &l_up);
	}

	out_rotationMatrix->Col0 = l_right;
	out_rotationMatrix->Col1 = l_up;
	out_rotationMatrix->Col2 = l_forward;
}

#endif

/************************ SEGMENT *************************/

#if 1

/* SEGMENT - Generic methods */

inline void Seg_Direction_VXF(
	const char* p_begin, const char* p_end,
	char* p_out, short int p_vectorElementCount)
{
	Vec_Min_Xf_Xf(p_end, p_begin, p_vectorElementCount, p_out);
	Vec_Normalize_Xf(p_out, p_vectorElementCount, p_out);
}


inline void Seg_ToVector_VXF(
	const char* p_begin, const char* p_end,
	char* p_out, short int p_vectorElementCount
)
{
	Vec_Min_Xf_Xf(p_end, p_begin, p_vectorElementCount, p_out);
}

inline void Seg_Mul_VXF_VXF(
	const char* p_begin, const char* p_end,
	const char* p_right,
	char* out_begin, char* out_end,
	short int p_vectorElementCount
)
{
	Vec_Mul_Xf_Xf(p_begin, p_vectorElementCount, p_right, out_begin);
	Vec_Mul_Xf_Xf(p_end, p_vectorElementCount, p_right, out_end);
}

inline void Seg_Mul_VXF_MXxXF(
	const char* p_begin, const char* p_end,
	const char* p_matrix,
	char* out_begin, char* out_end,
	const short int p_vectorElementCount,
	const size_t p_matrixLineSize,
	const size_t p_vectorSize
)
{
	Mat_Mul_MXxXf_MXxXf(p_matrix, p_begin, p_vectorElementCount, 1, p_matrixLineSize, p_vectorSize, out_begin);
	Mat_Mul_MXxXf_MXxXf(p_matrix, p_end, p_vectorElementCount, 1, p_matrixLineSize, p_vectorSize, out_end);
}

void Seg_Direction_V2F(const Segment_Vector2f_PTR p_segment, Vector2f_PTR p_out)
{
	Seg_Direction_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (char*)p_out->Points, 2);
};

void Seg_ToVector_V2F(const Segment_Vector2f_PTR p_segment, Vector2f_PTR p_out)
{
	Seg_ToVector_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (char*)p_out->Points, 2);
};

void Seg_Mul_V2F_V2F(const Segment_Vector2f_PTR p_segment, const Vector2f_PTR p_right, Segment_Vector2f_PTR p_out)
{
	Seg_Mul_VXF_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (const char*)p_right, (char*)p_out->Begin.Points, (char*)p_out->End.Points, 2);
};

void Seg_Direction_V3F(const Segment_Vector3f_PTR p_segment, Vector3f_PTR p_out)
{
	Seg_Direction_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (char*)p_out->Points, 3);
};
void Seg_ToVector_V3F(const Segment_Vector3f_PTR p_segment, Vector3f_PTR p_out)
{
	Seg_ToVector_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (char*)p_out->Points, 3);
};
void Seg_Mul_V3F_V3F(const Segment_Vector3f_PTR p_segment, const Vector3f_PTR p_right, Segment_Vector3f_PTR p_out)
{
	Seg_Mul_VXF_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (const char*)p_right, (char*)p_out->Begin.Points, (char*)p_out->End.Points, 4);
};
void Seg_Mul_V3F_M3F(const Segment_Vector3f_PTR p_segment, const Matrix3f_PTR p_right, Segment_Vector3f_PTR p_out)
{
	Seg_Mul_VXF_MXxXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (const char*)p_right, (char*)p_out->Begin.Points, (char*)p_out->End.Points, 3, sizeof(Vector3f), sizeof(Vector3f));
};


void Seg_Direction_V4F(const Segment_Vector4f_PTR p_segment, Vector4f_PTR p_out)
{
	Seg_Direction_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (char*)p_out->Points, 4);
};
void Seg_ToVector_V4F(const Segment_Vector4f_PTR p_segment, Vector4f_PTR p_out)
{
	Seg_ToVector_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (char*)p_out->Points, 3);
};
void Seg_Mul_V4F_V4F(const Segment_Vector4f_PTR p_segment, const Vector4f_PTR p_right, Segment_Vector4f_PTR p_out)
{
	Seg_Mul_VXF_VXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (const char*)p_right, (char*)p_out->Begin.Points, (char*)p_out->End.Points, 4);
};
void Seg_Mul_V4F_M4F(const Segment_Vector4f_PTR p_segment, const Matrix4f_PTR p_right, Segment_Vector4f_PTR p_out)
{
	Seg_Mul_VXF_MXxXF((const char*)p_segment->Begin.Points, (const char*)p_segment->End.Points, (const char*)p_right, (char*)p_out->Begin.Points, (char*)p_out->End.Points, 4, sizeof(Vector4f), sizeof(Vector4f));
};
#endif


/************************ BOX *************************/

#if 1
void Box_Build_F_ArrVec3F(BoxF_PTR p_box, Array_Vector3f_PTR p_points)
{
	// #GEN BOX_BUILD_ALROGITHM(for (size_t i = 0; i < p_points->Size; i++) { Vector3f_PTR l_point = &p_points->Memory[i];, }, p_points->Size);

	Vector3f l_approximateCenter = Vector3f_ZERO;
	{
		for (size_t i = 0; i < p_points->Size; i++)
		{
			Vector3f_PTR l_point = &p_points->Memory[i];
			Vec_Add_3f_3f(&l_approximateCenter, l_point, &l_approximateCenter);
		}
		Vec_Mul_3f_1f(&l_approximateCenter, 1.0f / p_points->Size, &l_approximateCenter);
	}
	Vector3f l_min = l_approximateCenter, l_max = l_approximateCenter;
	{
		for (size_t i = 0; i < p_points->Size; i++)
		{
			Vector3f_PTR l_point = &p_points->Memory[i];
			if (l_point->x <= l_min.x)
			{
				l_min.x = l_point->x;
			}
			if (l_point->y <= l_min.y)
			{
				l_min.y = l_point->y;
			}
			if (l_point->z <= l_min.z)
			{
				l_min.z = l_point->z;
			}
			if (l_point->x >= l_max.x)
			{
				l_max.x = l_point->x;
			}
			if (l_point->y >= l_max.y)
			{
				l_max.y = l_point->y;
			}
			if (l_point->z >= l_max.z)
			{
				l_max.z = l_point->z;
			}
		}
	}
	{
		p_box->Center = Vector3f_ZERO;
		Vec_Add_3f_3f(&p_box->Center, &l_min, &p_box->Center);
		Vec_Add_3f_3f(&p_box->Center, &l_max, &p_box->Center);
		Vec_Mul_3f_1f(&p_box->Center, 0.5f, &p_box->Center);
	}
	Box_Grow_F(p_box, &l_min);
	Box_Grow_F(p_box, &l_max);

};

void Box_ExtractPoints_F(const BoxF_PTR p_box, BoxFPoints_PTR p_out)
{

	p_out->Center = p_box->Center;
	Vector3f tmp_vec3;
	tmp_vec3 = (Vector3f){ p_box->Extend.x, p_box->Extend.y, p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->R_U_F);
	tmp_vec3 = (Vector3f){ p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->R_D_F);
	tmp_vec3 = (Vector3f){ p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->R_U_B);
	tmp_vec3 = (Vector3f){ p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->R_D_B);
	tmp_vec3 = (Vector3f){ -p_box->Extend.x, p_box->Extend.y, p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->L_U_F);
	tmp_vec3 = (Vector3f){ -p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->L_D_F);
	tmp_vec3 = (Vector3f){ -p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->L_U_B);
	tmp_vec3 = (Vector3f){ -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, &p_out->L_D_B);
};

void Box_ExtractMinMax_F(const BoxF_PTR p_box, Vector3f_PTR out_min, Vector3f_PTR out_max)
{
	Vector3f tmp_vec3;
	tmp_vec3 = (Vector3f){ -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, out_min);
	tmp_vec3 = (Vector3f){ p_box->Extend.x, p_box->Extend.y, p_box->Extend.z }; Vec_Add_3f_3f(&p_box->Center, &tmp_vec3, out_max);
};

void BoxPoints_Mul_F_M4F(const BoxFPoints_PTR p_boxPoints, const Matrix4f_PTR p_matrix, BoxFPoints_PTR p_out)
{
	Vector4f tmp_vec4_0, tmp_vec4_1;
	for (short int i = 0; i < 9; i++)
	{
		tmp_vec4_0 = (Vector4f){ .Vec3 = p_boxPoints->Points[i], .Vec3_w = 1.0f };
		Mat_Mul_M4F_V4F(p_matrix, &tmp_vec4_0, &tmp_vec4_1);
		p_out->Points[i] = tmp_vec4_1.Vec3;
	}
}

#endif

/************************ Colors *************************/

#if 1
void Color_Convert_3F_3C(Color3f_PTR p_color3f, Color3c_PTR p_color3c)
{
	p_color3c->r = (char)(Math_clamp01f(p_color3f->r) * 255.0f);
	p_color3c->g = (char)(Math_clamp01f(p_color3f->g) * 255.0f);
	p_color3c->b = (char)(Math_clamp01f(p_color3f->b) * 255.0f);
};
#endif

/************************ Plane *************************/

#if 1


void Plane_Build_EquationFactors(const float p_a, const float p_b, const float p_c, const float p_d, Plane_PTR p_out)
{
	p_out->Normal = (Vector3f){ p_a, p_b, p_c };
	Vec_Normalize_3f(&p_out->Normal, &p_out->Normal);
	p_out->Point = (Vector3f){ 0.0f, 0.0f, p_d / p_c };
};

void Plane_Build_Normal_Point(const Vector3f_PTR p_normal, const Vector3f_PTR p_point, Plane_PTR p_out)
{
	p_out->Normal = *p_normal;
	p_out->Point = *p_point;
}

void  Plane_Build_3Points(const Vector3f_PTR p_0, const  Vector3f_PTR p_1, const  Vector3f_PTR p_2, Plane_PTR p_out)
{
	Vector3f l_01; Vec_Min_3f_3f(p_1, p_0, &l_01);
	Vector3f l_02; Vec_Min_3f_3f(p_2, p_0, &l_02);

	Vec_Cross_3f(&l_01, &l_02, &p_out->Normal);
	Vec_Normalize_3f(&p_out->Normal, &p_out->Normal);
	p_out->Point = *p_0;
};

float Plane_MinimumDistance_V3F(const Plane_PTR p_plane, const Vector3f_PTR p_point)
{
	Vector3f l_d;
	Vec_Min_3f_3f(p_point, &p_plane->Point, &l_d);
	return fabsf(Vec_Dot_3f(&l_d, &p_plane->Normal));
};

void Plane_ProjectPoint(const Plane_PTR p_plane, const Vector3f_PTR p_point, Vector3f_PTR out_projectedPoint)
{
	Vector3f l_displacement; Vec_Min_3f_3f(p_point, &p_plane->Point, &l_displacement);
	Vector3f l_displacement_alongPlaneNormal; Vec_Project_3f(&l_displacement, &p_plane->Normal, &l_displacement_alongPlaneNormal);
	Vector3f l_displacement_alongPlane; Vec_Min_3f_3f(&l_displacement, &l_displacement_alongPlaneNormal, &l_displacement_alongPlane);
	Vec_Add_3f_3f(&p_plane->Point, &l_displacement_alongPlane, out_projectedPoint);
};

#endif


/************************ Frustum *************************/

#if 1

// Reference  http://deptinfo.unice.fr/twiki/pub/Minfo03/AlgosJeux3DMariani/planeextraction.pdf
void Frustum_ExtractFromProjection(const Matrix4f_PTR p_projection, Frustum_PTR out_frustum)
{
	//TODO - Having a more direct way instead of calling inv of projection ? (more efficient - see link above)

	Matrix4f l_projection_inverted;
	Mat_Inv_M4F(p_projection, &l_projection_inverted);

	Vector4f l_right_up_far, l_right_up_near, l_right_bottom_far, l_right_bottom_near;
	Vector4f l_left_up_far, l_left_up_near, l_left_bottom_far, l_left_bottom_near;

	Vector4f tmp_vec4_0 = { 1.0f, 1.0f, 1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_right_up_far);

	tmp_vec4_0 = (Vector4f){ 1.0f, 1.0f, -1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_right_up_near);

	tmp_vec4_0 = (Vector4f){ 1.0f, -1.0f, 1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_right_bottom_far);

	tmp_vec4_0 = (Vector4f){ 1.0f, -1.0f, -1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_right_bottom_near);


	tmp_vec4_0 = (Vector4f){ -1.0f, 1.0f, 1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_left_up_far);

	tmp_vec4_0 = (Vector4f){ -1.0f, 1.0f, -1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_left_up_near);

	tmp_vec4_0 = (Vector4f){ -1.0f, -1.0f, 1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_left_bottom_far);

	tmp_vec4_0 = (Vector4f){ -1.0f, -1.0f, -1.0f, 1.0f };
	Mat_Mul_M4F_V4F_Homogeneous(&l_projection_inverted, &tmp_vec4_0, &l_left_bottom_near);

	// /!\ Order of point matters to make sure that normals are pointing inward
	Plane_Build_3Points((Vector3f_PTR)&l_left_bottom_near, (Vector3f_PTR)&l_left_up_near, (Vector3f_PTR)&l_left_bottom_far, &out_frustum->Left);
	Plane_Build_3Points((Vector3f_PTR)&l_right_bottom_near, (Vector3f_PTR)&l_right_bottom_far, (Vector3f_PTR)&l_right_up_near, &out_frustum->Right);

	Plane_Build_3Points((Vector3f_PTR)&l_left_bottom_near, (Vector3f_PTR)&l_left_bottom_far, (Vector3f_PTR)&l_right_bottom_near, &out_frustum->Bottom);
	Plane_Build_3Points((Vector3f_PTR)&l_left_up_near, (Vector3f_PTR)&l_right_up_near, (Vector3f_PTR)&l_left_up_far, &out_frustum->Up);

	Plane_Build_3Points((Vector3f_PTR)&l_left_bottom_near, (Vector3f_PTR)&l_right_bottom_near, (Vector3f_PTR)&l_left_up_near, &out_frustum->Near);
	Plane_Build_3Points((Vector3f_PTR)&l_left_bottom_far, (Vector3f_PTR)&l_left_up_far, (Vector3f_PTR)&l_right_bottom_far, &out_frustum->Far);
};

#endif

/************************ CLIP *************************/

#if 1

typedef short int CLIPPOSITION;
const CLIPPOSITION CLIPPOSITION_INSIDE = 0;
const CLIPPOSITION CLIPPOSITION_UP = 1;
const CLIPPOSITION CLIPPOSITION_DOWN = 2;
const CLIPPOSITION CLIPPOSITION_LEFT = 4;
const CLIPPOSITION CLIPPOSITION_RIGHT = 8;
const CLIPPOSITION CLIPPOSITION_UP_LEFT = 1 & 4;
const CLIPPOSITION CLIPPOSITION_UP_RIGHT = 1 & 8;
const CLIPPOSITION CLIPPOSITION_DOWN_LEFT = 2 & 4;
const CLIPPOSITION CLIPPOSITION_DOWN_RIGHT = 2 & 8;

CLIPPOSITION Rect_CalculateClipPosition(const Vector2i_PTR p_point, const Recti_PTR p_clipRect)
{
	CLIPPOSITION l_pos = CLIPPOSITION_INSIDE;

	if (p_point->y > p_clipRect->Max.y)
	{
		l_pos |= CLIPPOSITION_UP;
	}
	else if (p_point->y < p_clipRect->Min.y)
	{
		l_pos |= CLIPPOSITION_DOWN;
	}

	if (p_point->x > p_clipRect->Max.x)
	{
		l_pos |= CLIPPOSITION_RIGHT;
	}
	else if (p_point->x < p_clipRect->Min.x)
	{
		l_pos |= CLIPPOSITION_LEFT;
	}

	return l_pos;
}

bool Rect_ClipSegment_Int(const Vector2i_PTR in_clippedSegment_begin, const Vector2i_PTR in_clippedSegment_end,
	const Recti_PTR p_clippedRect, Vector2i_PTR out_clippedSegment_begin, Vector2i_PTR out_clippedSegment_end)
{
	CLIPPOSITION l_beginClip = Rect_CalculateClipPosition(in_clippedSegment_begin, p_clippedRect);
	CLIPPOSITION l_endClip = Rect_CalculateClipPosition(in_clippedSegment_end, p_clippedRect);

	*out_clippedSegment_begin = *in_clippedSegment_begin;
	*out_clippedSegment_end = *in_clippedSegment_end;

	if ((l_beginClip != CLIPPOSITION_INSIDE) || (l_endClip != CLIPPOSITION_INSIDE))
	{
		CLIPPOSITION l_beginClippedClip = l_beginClip;
		CLIPPOSITION l_endClippedClip = l_endClip;

		// Not the same region
		while (true)
		{
			if ((l_beginClippedClip == CLIPPOSITION_INSIDE) && (l_endClippedClip == CLIPPOSITION_INSIDE))
			{
				break;
			}
			// Outside of the rect
			else if (l_beginClippedClip & l_endClippedClip)
			{
				return false;
			}
			else
			{
				short int l_selectedPoint = 0;
				CLIPPOSITION l_selectedClip;
				if (l_beginClippedClip != CLIPPOSITION_INSIDE)
				{
					l_selectedPoint = 0;
					l_selectedClip = l_beginClippedClip;
				}
				else if (l_endClippedClip != CLIPPOSITION_INSIDE)
				{
					l_selectedPoint = 1;
					l_selectedClip = l_endClippedClip;
				}

				int l_x, l_y;

				if (l_selectedClip & CLIPPOSITION_UP)
				{
					l_y = p_clippedRect->Max.y;
					// deltaX = slope * (fixed position diff)
					l_x = out_clippedSegment_begin->x + (int)(((float)(out_clippedSegment_end->x - out_clippedSegment_begin->x) / (out_clippedSegment_end->y - out_clippedSegment_begin->y)) * (l_y - out_clippedSegment_begin->y));
				}
				else if (l_selectedClip & CLIPPOSITION_DOWN)
				{
					l_y = p_clippedRect->Min.y;
					l_x = out_clippedSegment_begin->x + (int)(((float)(out_clippedSegment_end->x - out_clippedSegment_begin->x) / (out_clippedSegment_end->y - out_clippedSegment_begin->y)) * (l_y - out_clippedSegment_begin->y));
				}
				else if (l_selectedClip & CLIPPOSITION_RIGHT)
				{
					l_x = p_clippedRect->Max.x;
					l_y = out_clippedSegment_begin->y + (int)(((float)(out_clippedSegment_end->y - out_clippedSegment_begin->y) / (out_clippedSegment_end->x - out_clippedSegment_begin->x)) * (l_x - out_clippedSegment_begin->x));
				}
				else if (l_selectedClip & CLIPPOSITION_LEFT)
				{
					l_x = p_clippedRect->Min.x;
					l_y = out_clippedSegment_begin->y + (int)(((float)(out_clippedSegment_end->y - out_clippedSegment_begin->y) / (out_clippedSegment_end->x - out_clippedSegment_begin->x)) * (l_x - out_clippedSegment_begin->x));
				}

				if (l_selectedPoint == 0)
				{
					out_clippedSegment_begin->x = l_x;
					out_clippedSegment_begin->y = l_y;
					l_beginClippedClip = Rect_CalculateClipPosition(out_clippedSegment_begin, p_clippedRect);
				}
				else
				{
					out_clippedSegment_end->x = l_x;
					out_clippedSegment_end->y = l_y;
					l_endClippedClip = Rect_CalculateClipPosition(out_clippedSegment_end, p_clippedRect);
				}
			}
		}

	}

	*out_clippedSegment_begin = (Vector2i){ out_clippedSegment_begin->x, out_clippedSegment_begin->y };
	*out_clippedSegment_end = (Vector2i){ out_clippedSegment_end->x , out_clippedSegment_end->y };

	return true;
};

void Rect_ClipPoint_Int(const Recti_PTR p_clippedRect, Vector2i_PTR p_point)
{
	if (p_point->x > p_clippedRect->Max.x) { p_point->x = p_clippedRect->Max.x; }
	if (p_point->x < p_clippedRect->Min.x) { p_point->x = p_clippedRect->Min.x; }
	if (p_point->y > p_clippedRect->Max.y) { p_point->y = p_clippedRect->Max.y; }
	if (p_point->y < p_clippedRect->Min.y) { p_point->y = p_clippedRect->Min.y; }
};

#endif


/************************ WINDOW SIZE *************************/

#if 1

void WindowSize_GraphicsAPIToPixel(const WindowSize* p_windowSize, float p_x, float p_y, int* out_x, int* out_y)
{
	//TODO -> Disable rounding ?
	*out_x = (int)roundf(((p_x * p_windowSize->HalfWidth) + p_windowSize->HalfWidth));
	*out_y = (int)roundf(((p_y * p_windowSize->HalfHeight) + p_windowSize->HalfHeight));
};

void WindowSize_PixelToGraphicsAPI(const WindowSize* p_windowSize, int p_x, int p_y, float* out_x, float* out_y)
{
	*out_x = (p_windowSize->TwoOnWidth * p_x) - 1.0f;
	*out_y = (p_windowSize->TwoOnHeight * p_y) - 1.0f;
};

#endif