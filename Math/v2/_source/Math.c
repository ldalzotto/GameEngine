#include "VectorC.h"
#include "QuaternionC.h"
#include "MatrixC.h"

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

bool Vec_Equals_4f(const VECTOR4F_PTR p_left, const VECTOR4F_PTR p_right)
{
	return Equals_float(&p_left->x, &p_right->x)
		&& Equals_float(&p_left->y, &p_right->y)
		&& Equals_float(&p_left->z, &p_right->z)
		&& Equals_float(&p_left->w, &p_right->w);
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

void Vec_Rotate_3f(const VECTOR3F_PTR p_vector, const QUATERNION4F_PTR p_rotation, VECTOR3F_PTR p_out)
{
	QUATERNION4F l_vectorAsQuat;
	Quat_Build_3f_1f(p_vector, 0.0f, &l_vectorAsQuat);

	QUATERNION4F l_rotatedVector;
	Quat_Mul(p_rotation, &l_vectorAsQuat, &l_rotatedVector);
	{
		QUATERNION4F l_tmp;
		Quat_conjugate(p_rotation, &l_tmp);

		QUATERNION4F l_rotatedVector_cpy;
		memcpy(l_rotatedVector_cpy, l_rotatedVector, sizeof(QUATERNION4F));
		Quat_Mul(&l_rotatedVector_cpy, &l_tmp, &l_rotatedVector);
	}

	Vec_Normalize_3f(&l_rotatedVector.Vec, p_out);
};


/* QUATERNION */

bool Quat_Equals(const QUATERNION4F_PTR p_left, const QUATERNION4F_PTR p_right)
{
	return Vec_Equals_4f(&p_left->Points, &p_right->Points);
};

void Quat_Build_4f(const VECTOR4F_PTR p_points, QUATERNION4F_PTR p_out)
{
	p_out->Points = *p_points;
};

void Quat_Build_3f_1f(const VECTOR3F_PTR p_vec, float p_scal, QUATERNION4F_PTR p_out)
{
	p_out->Vec = *p_vec;
	p_out->Scal = p_scal;
};

/* QUATERNION - OPERATION */

void Quat_Normalize(const QUATERNION4F_PTR p_quat, QUATERNION4F_PTR p_out)
{
	Vec_Mul_4f_1f(&p_quat->Points, 1.0f / Vec_Length_4f(&p_quat->Points), (VECTOR4F_PTR)p_out);
};

void Quat_Mul(const QUATERNION4F_PTR p_left, const QUATERNION4F_PTR p_right, QUATERNION4F_PTR p_out)
{
	Vec_Mul_3f_1f(p_left, p_right->w, p_out);
	{
		VECTOR3F l_tmp;
		Vec_Mul_3f_1f(p_right, p_left->w, &l_tmp);
		Vec_Add_3f_3f(p_out, &l_tmp, p_out);

		Vec_Cross_3f(p_left, p_right, &l_tmp);
		Vec_Add_3f_3f(p_out, &l_tmp, p_out);
	}

	p_out->w = (p_left->w * p_right->w) - Vec_Dot_3f(p_left, p_right);
	Quat_Normalize(p_out, p_out);
};

void Quat_Cross(const QUATERNION4F_PTR p_left, const  QUATERNION4F_PTR p_right, QUATERNION4F_PTR p_out)
{
	VECTOR3F l_rotatedLeft;
	Vec_Rotate_3f(&VECTOR3F_FORWARD, p_left, &l_rotatedLeft);

	VECTOR3F l_rotatedRight;
	Vec_Rotate_3f(&VECTOR3F_FORWARD, p_right, &l_rotatedRight);

	VECTOR3F l_crossResult;
	Vec_Cross_3f(&l_rotatedLeft, &l_rotatedRight, &l_crossResult);
	Quat_RotateAround(&l_crossResult, 0.0f, p_out);
};

void Quat_ExtractAxis(const QUATERNION4F_PTR quat, float out_axis[3][3])
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
	out_axis[0][0] = 1 - (2 * l_qyy) - (2 * l_qzz);
	out_axis[0][1] = (2 * l_qxy) + (2 * l_qzw);
	out_axis[0][2] = (2 * l_qxz) - (2 * l_qyw);

	//UP
	out_axis[1][0] = (2 * l_qxy) - (2 * l_qzw);
	out_axis[1][1] = 1 - (2 * l_qxx) - (2 * l_qzz);
	out_axis[1][2] = (2 * l_qyz) + (2 * l_qxw);

	//Forward
	out_axis[2][0] = (2 * l_qxz) + (2 * l_qyw);
	out_axis[2][1] = (2 * l_qyz) - (2 * l_qxw);
	out_axis[2][2] = 1 - (2 * l_qxx) - (2 * l_qyy);

	Vec_Normalize_3f((const VECTOR3F_PTR)out_axis[0], (VECTOR3F_PTR)out_axis[0]);
	Vec_Normalize_3f((const VECTOR3F_PTR)out_axis[1], (VECTOR3F_PTR)out_axis[1]);
	Vec_Normalize_3f((const VECTOR3F_PTR)out_axis[2], (VECTOR3F_PTR)out_axis[2]);
};


/* QUATERNIOn - BUILD */

void Quat_FromDirection(const VECTOR3F_PTR p_vec, QUATERNION4F_PTR p_out)
{
	float l_angle = Vec_Angle_3f(&VECTOR3F_FORWARD, p_vec);
	Vec_Mul_3f_1f(p_vec, sinf(l_angle * 0.5f), p_out);
	p_out->w = cosf(l_angle * 0.5f);
};

void Quat_FromAxis(const float p_axis[3][3], QUATERNION4F_PTR p_out)
{
	const float* l_right = p_axis[0];
	const float* l_up = p_axis[1];
	const float* l_forward = p_axis[2];

	// We calculate the four square roots and get the higher one.
	float qxDiag = fmaxf(1 + l_right[0] - l_up[1] - l_forward[2], 0.0f);
	float qyDiag = fmaxf(1 + l_up[1] - l_right[0] - l_forward[2], 0.0f);
	float qzDiag = fmaxf(1 + l_forward[2] - l_right[0] - l_up[1], 0.0f);
	float qwDiag = fmaxf(1 + l_right[0] + l_up[1] + l_forward[2], 0.0f);

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
		p_out->y = (l_right[1] + l_up[0]) * mult;
		p_out->z = (l_forward[0] + l_right[2]) * mult;
		p_out->w = (l_up[2] - l_forward[1]) * mult;
	}
	break;
	case 1:
	{
		p_out->x = (l_right[1] + l_up[0]) * mult;
		p_out->y = l_biggestDiagonalValue;
		p_out->z = (l_up[2] + l_forward[1]) * mult;
		p_out->w = (l_forward[0] - l_right[2]) * mult;
	}
	break;
	case 2:
	{
		p_out->x = (l_forward[0] + l_right[2]) * mult;
		p_out->y = (l_up[2] + l_forward[1]) * mult;
		p_out->z = l_biggestDiagonalValue;
		p_out->w = (l_right[1] - l_up[0]) * mult;
	}
	break;
	case 3:
	{
		p_out->x = (l_up[2] - l_forward[1]) * mult;
		p_out->y = (l_forward[0] - l_right[2]) * mult;
		p_out->z = (l_right[1] - l_up[0]) * mult;
		p_out->w = l_biggestDiagonalValue;
	}
	break;
	}

};

void Quat_FromEulerAngle(const VECTOR3F_PTR p_eulerAngle, QUATERNION4F_PTR p_out)
{
	{
		QUATERNION4F l_yaw;
		Quat_RotateAround(&VECTOR3F_UP, p_eulerAngle->y, &l_yaw);
		p_out->x = 0.0f; p_out->y = 0.0f; p_out->z = 0.0f; p_out->w = 1.0f;
		QUATERNION4F l_tmp;
		memcpy(&l_tmp, p_out, sizeof(QUATERNION4F));
		Quat_Mul(&l_tmp, &l_yaw, p_out);
	}
	{
		QUATERNION4F l_pitch;
		Quat_RotateAround(&VECTOR3F_RIGHT, p_eulerAngle->x, &l_pitch);
		QUATERNION4F l_tmp;
		memcpy(&l_tmp, p_out, sizeof(QUATERNION4F));
		Quat_Mul(&l_tmp, &l_pitch, p_out);
	}
	{
		QUATERNION4F l_roll;
		Quat_RotateAround(&VECTOR3F_FORWARD, p_eulerAngle->z, &l_roll);
		QUATERNION4F l_tmp;
		memcpy(&l_tmp, p_out, sizeof(QUATERNION4F));
		Quat_Mul(&l_tmp, &l_roll, p_out);
	}

};

void Quat_FromTo(const VECTOR3F_PTR p_from, const  VECTOR3F_PTR p_to, QUATERNION4F_PTR p_out)
{
	float l_angle = Vec_Angle_3f(p_from, p_to);
	float l_rotationAxis[3];
	Vec_Cross_3f(p_from, p_to, l_rotationAxis);
	Quat_RotateAround(l_rotationAxis, l_angle, p_out);
};

void Quat_conjugate(const QUATERNION4F_PTR p_quat, QUATERNION4F_PTR p_out)
{
	Vec_Mul_3f_1f(&p_quat->Vec, -1.0f, &p_out->Vec);
	p_out->Scal = p_quat->Scal;
};


void Quat_RotateAround(const VECTOR3F_PTR p_axis, const  float p_angle, QUATERNION4F_PTR out_quat)
{
	Vec_Mul_3f_1f(p_axis, sinf(p_angle * 0.5f), out_quat);
	out_quat->w = cosf(p_angle * 0.5f);
};


/* MATRIX */

/*
	inline float RMatrix_multiplication_line_column_v3(const RMatrix& p_left, const size_t p_left_elementSize, const RMatrix& p_right, const size_t p_right_elementSize, int p_leftLineIndex, int p_rightColumnIndex)
	{
		float l_return = 0.0f;
		for (short int i = 0; i < p_left.ColumnCount; i++)
		{
			l_return += (*(float*)RMatrix_float_getAddresFromRaw_v3(p_left, i, p_leftLineIndex, p_left_elementSize) * *(float*)RMatrix_float_getAddresFromRaw_v3(p_right, p_rightColumnIndex, i, p_right_elementSize));
		}
		return l_return;
	};

	inline void RMatrix_multiplication_matrix_matrix_v3(const RMatrix& p_left, const size_t p_left_elementSize, const RMatrix& p_right, const size_t p_right_elementSize, RMatrix& p_out)
	{
		for (int i = 0; i < p_left.LineCount; i++)
		{
			for (int j = 0; j < p_right.ColumnCount; j++)
			{
				*(float*)RMatrix_float_getAddresFromRaw_v3(p_out, j, i, p_left_elementSize) = RMatrix_multiplication_line_column_v3(p_left, p_left_elementSize, p_right, p_right_elementSize, i, j);
			}
		}
	}

*/

inline float Mat_Mul_line_column_Xf(const float* p_left_line, const float* p_right_column, const short int p_leftColumnCount, const size_t p_fullColumnSize)
{
	float l_return = 0.0f;
	char* l_left_lineCursor = p_left_line;
	char* l_right_columnCursor = p_right_column;

	for (short int i = 0; i < p_leftColumnCount; i++)
	{
		l_return += (*((float*)l_left_lineCursor) * *((float*)l_right_columnCursor));

		l_left_lineCursor += p_fullColumnSize;
		l_right_columnCursor += sizeof(float);
	}

	return l_return;
};

inline void Mat_Mul_MXxXf_MXxXf(const float* p_left, const float* p_right,
	const short int p_left_lineCount, const short int p_right_columnCount,
	const size_t p_left_lineSize, const size_t p_right_columnSize,
	float* p_out)
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
	char* l_mat_cursor = p_mat;

	for (short int i = 0; i < p_matrix_elementCount; i++)
	{
		*(float*)l_mat_cursor *= p_value;
		l_mat_cursor += sizeof(float);
	}
};

inline float Mat_Det_M4F(const MATRIX4F_PTR p_mat, const short int p_colIndex, const short int p_lineIndex)
{
	MATRIX3F l_mat3x3 = { 0 };
	{

		char* l_mat3_cursor = l_mat3x3.Points;
		char* l_mat_cursor = p_mat->Points;

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
				l_mat_cursor += sizeof(VECTOR4F);
			}
		}
	}

	return
		(l_mat3x3._00 * ((l_mat3x3._11 * l_mat3x3._22) - (l_mat3x3._12 * l_mat3x3._21))) +
		(l_mat3x3._10 * ((l_mat3x3._21 * l_mat3x3._02) - (l_mat3x3._22 * l_mat3x3._01))) +
		(l_mat3x3._20 * ((l_mat3x3._01 * l_mat3x3._12) - (l_mat3x3._02 * l_mat3x3._11)));
};


void Mat_Mul_M4F_M4F(const MATRIX4F_PTR p_left, const MATRIX4F_PTR p_right, MATRIX4F_PTR p_out)
{
	Mat_Mul_MXxXf_MXxXf(p_left, p_right, 4, 4, sizeof(VECTOR4F), sizeof(VECTOR4F), p_out);
};

void Mat_Mul_M4F_V4F(const MATRIX4F_PTR p_left, const VECTOR4F_PTR p_right, VECTOR4F_PTR p_out)
{
	Mat_Mul_MXxXf_MXxXf(p_left, p_right, 4, 1, sizeof(VECTOR4F), sizeof(VECTOR4F), p_out);
};

void Mat_Mul_M3F_V3F(const MATRIX3F_PTR p_left, const VECTOR3F_PTR p_right, VECTOR3F_PTR p_out)
{
	Mat_Mul_MXxXf_MXxXf(p_left, p_right, 3, 1, sizeof(VECTOR3F), sizeof(VECTOR3F), p_out);
};

void Mat_Inv_M4F(const MATRIX4F_PTR p_matrix, MATRIX4F_PTR p_out)
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

void Mat_Translation_M4F(const VECTOR3F_PTR p_translation, MATRIX4F_PTR out_mat)
{
	out_mat->Col3.Vec3 = *p_translation;
};

void Mat_RotationAxis_M4F(const MATRIX3F_PTR p_axis, MATRIX4F_PTR out_mat)
{
	out_mat->Col0.Vec3 = p_axis->Col0;
	out_mat->Col1.Vec3 = p_axis->Col1;
	out_mat->Col2.Vec3 = p_axis->Col2;
};

void Mat_RotationSeparatedAxis_M4F(const VECTOR3F_PTR p_right, const VECTOR3F_PTR p_up, const VECTOR3F_PTR p_forward, MATRIX4F_PTR out_mat)
{
	out_mat->Col0.Vec3 = *p_right;
	out_mat->Col1.Vec3 = *p_up;
	out_mat->Col2.Vec3 = *p_forward;
};

void Mat_Scale_M4F(const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_mat)
{
	out_mat->_00 *= p_scale->x;
	out_mat->_11 *= p_scale->y;
	out_mat->_22 *= p_scale->z;
};

void Mat_TRS_Axis_M4F(const VECTOR3F_PTR p_position, const MATRIX3F_PTR p_axis, const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_TRS)
{
	*out_TRS = MATRIX4F_IDENTITYF;
	Mat_Translation_M4F(p_position, out_TRS);
	Mat_RotationAxis_M4F(p_axis, out_TRS);
	Mat_Scale_M4F(p_scale, out_TRS);
};

void Mat_TRS_Quat_M4F(const VECTOR3F_PTR p_position, const QUATERNION4F_PTR p_quat, const VECTOR3F_PTR p_scale, MATRIX4F_PTR out_TRS)
{
	MATRIX3F l_axis; Quat_ExtractAxis(p_quat, l_axis.Points);
	Mat_TRS_Axis_M4F(p_position, &l_axis, p_scale, out_TRS);
};