#include "Quaternion.h"

#include <math.h>
#include "v2/Vector/Vector.h"

namespace _MathV2
{
	void RQuaternion_build(float p_vec[3], float p_scal, float p_out[4])
	{
		p_out[0] = p_vec[0];
		p_out[1] = p_vec[1];
		p_out[2] = p_vec[2];
		p_out[3] = p_scal;
	};

	void RQuaternion_fromDirection(float p_vec[3], float p_out[4])
	{
		float l_angle = RVector_3_angle(FORWARD_arr, p_vec);
		RVector_3_mul(p_vec, sinf(l_angle * 0.5f), p_out);
		p_out[3] = cosf(l_angle * 0.5f);
	};

	void RQuaternion_fromAxis(float p_axis[3][3], float p_out[4])
	{
		float* l_right = p_axis[0];
		float* l_up = p_axis[1];
		float* l_forward = p_axis[2];

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
			p_out[0] = l_biggestDiagonalValue;
			p_out[1] = (l_right[1] + l_up[0]) * mult;
			p_out[2] = (l_forward[0] + l_right[2]) * mult;
			p_out[3] = (l_up[2] - l_forward[1]) * mult;
		}
		break;
		case 1:
		{
			p_out[0] = (l_right[1] + l_up[0]) * mult;
			p_out[1] = l_biggestDiagonalValue;
			p_out[2] = (l_up[2] + l_forward[1]) * mult;
			p_out[3] = (l_forward[0] - l_right[2]) * mult;
		}
		break;
		case 2:
		{
			p_out[0] = (l_forward[0] + l_right[2]) * mult;
			p_out[1] = (l_up[2] + l_forward[1]) * mult;
			p_out[2] = l_biggestDiagonalValue;
			p_out[3] = (l_right[1] - l_up[0]) * mult;
		}
		break;
		case 3:
		{
			p_out[0] = (l_up[2] - l_forward[1]) * mult;
			p_out[1] = (l_forward[0] - l_right[2]) * mult;
			p_out[2] = (l_right[1] - l_up[0]) * mult;
			p_out[3] = l_biggestDiagonalValue;
		}
		break;
		}

	};

	/*
			T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
		T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

		int biggestIndex = 0;
		T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		T biggestVal = sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
		T mult = static_cast<T>(0.25) / biggestVal;

		switch(biggestIndex)
		{
		case 0:
			return qua<T, Q>(biggestVal, (m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult);
		case 1:
			return qua<T, Q>((m[1][2] - m[2][1]) * mult, biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult);
		case 2:
			return qua<T, Q>((m[2][0] - m[0][2]) * mult, (m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult);
		case 3:
			return qua<T, Q>((m[0][1] - m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal);
		default: // Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
			assert(false);
			return qua<T, Q>(1, 0, 0, 0);
		}
	*/

	void RQuaternion_fromTo(float p_from[3], float p_to[3], float p_out[4])
	{
		float l_angle = RVector_3_angle(p_from, p_to);
		float l_rotationAxis[3];
		RVector_3_cross(p_from, p_to, l_rotationAxis);
		RQuaternion_rotateAround(l_rotationAxis, l_angle, p_out);
	};

	void RQuaternion_conjugate(float p_quat[4], float p_out[4])
	{
		RVector_3_mul(p_quat, -1.0f, p_out);
		p_out[3] = p_quat[3];
	};

	void RQuaternion_normalize(float p_quat[4], float p_out[4])
	{
		RVector_4_mul(p_quat, 1 / RVector_4_length(p_quat), p_out);
	};

	void RQuaternion_mul(float p_left[4], float p_right[4], float p_out[4])
	{
		RVector_3_mul(p_left, p_right[3], p_out);
		{
			float l_tmp[3];
			RVector_3_mul(p_right, p_left[3], l_tmp);
			RVector_3_add(p_out, l_tmp, p_out);

			RVector_3_cross(p_left, p_right, l_tmp);
			RVector_3_add(p_out, l_tmp, p_out);
		}

		p_out[3] = (p_left[3] * p_right[3]) - RVector_3_dot(p_left, p_right);
		RQuaternion_normalize(p_out, p_out);
	};

	void RQuaternion_cross(float p_left[4], float p_right[4], float p_out[4])
	{
		float l_rotatedLeft[3];
		RVector_3_rotate(FORWARD_arr, p_left, l_rotatedLeft);

		float l_rotatedRight[3];
		RVector_3_rotate(FORWARD_arr, p_right, l_rotatedRight);

		float l_crossResult[3];
		RVector_3_cross(l_rotatedLeft, l_rotatedRight, l_crossResult);
		RQuaternion_rotateAround(l_crossResult, 0.0f, p_out);
	};

	void RQuaternion_rotateAround(float p_axis[3], float p_angle, float out_quat[4])
	{
		RVector_3_mul(p_axis, sinf(p_angle * 0.5f), out_quat);
		out_quat[3] = cosf(p_angle * 0.5f);
	};

	void RQuaternion_extractAxis(float quat[4], float out_axis[3][3])
	{
		RVector_3_rotate(RIGHT_arr, quat, out_axis[0]);
		RVector_3_rotate(UP_arr, quat, out_axis[1]);
		RVector_3_rotate(FORWARD_arr, quat, out_axis[2]);

		RVector_3_normalize(out_axis[0], out_axis[0]);
		RVector_3_normalize(out_axis[1], out_axis[1]);
		RVector_3_normalize(out_axis[2], out_axis[2]);
	};
}