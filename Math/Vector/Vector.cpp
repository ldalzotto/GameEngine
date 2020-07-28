#include "Vector.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "Quaternion/Quaternion.h"

namespace _Math
{

	inline void RVector_mul(float* p_left, int p_left_nb, float p_value, float* p_out)
	{
		for (int i = 0; i < p_left_nb; i++)
		{
			p_out[i] = p_left[i] * p_value;
		}
	};

	void RVector_3_add(float p_left[3], float p_right[3], float p_out[3])
	{
		p_out[0] = p_left[0] + p_right[0];
		p_out[1] = p_left[1] + p_right[1];
		p_out[2] = p_left[2] + p_right[2];
	};

	void RVector_3_mul(float p_left[3], float p_right, float p_out[3])
	{
		RVector_mul(p_left, 3, p_right, p_out);
	};

	void RVector_4_mul(float p_left[4], float p_right, float p_out[4])
	{
		RVector_mul(p_left, 4, p_right, p_out);
	};

	float RVector_3_length(float p_vec[3])
	{
		return
			sqrtf(RVector_3_dot(p_vec, p_vec));
	};

	float RVector_4_length(float p_vec[4])
	{
		return
			sqrtf((p_vec[0] * p_vec[0]) +
				(p_vec[1] * p_vec[1]) +
				(p_vec[2] * p_vec[2]) +
				(p_vec[3] * p_vec[3]));
	};

	void RVector_3_normalize(float p_vec[3], float p_out[3])
	{
		float l_length = RVector_3_length(p_vec);
		p_out[0] = p_vec[0] / l_length;
		p_out[1] = p_vec[1] / l_length;
		p_out[2] = p_vec[2] / l_length;
	};

	float RVector_3_dot(float p_left[3], float p_right[3])
	{
		return
			(p_left[0] * p_right[0]) +
			(p_left[1] * p_right[1]) +
			(p_left[2] * p_right[2]);
	};

	void RVector_3_cross(float p_left[3], float p_right[3], float p_out[3])
	{
		p_out[0] = (p_left[1] * p_right[2]) - (p_left[2] * p_right[1]);
		p_out[1] = (p_left[2] * p_right[0]) - (p_left[0] * p_right[2]);
		p_out[2] = (p_left[0] * p_right[1]) - (p_left[1] * p_right[0]);
	};

	float RVector_3_angle(float p_begin[3], float p_end[3])
	{
		return acosf(
			RVector_3_dot(p_begin, p_end) / (RVector_3_length(p_begin) * RVector_3_length(p_end))
		);
	};

	void RVector_3_rotate(float p_vector[3], float p_rotation[4], float p_out[3])
	{
		float l_vectorAsQuat[4];
		RQuaternion_build(p_vector, 0.0f, l_vectorAsQuat);
		
		float l_rotatedVector[4];
		RQuaternion_mul(p_rotation, l_vectorAsQuat, l_rotatedVector);
		{
			float l_tmp[4];
			RQuaternion_conjugate(p_rotation, l_tmp);

			float l_rotatedVector_cpy[4]; 
			memcpy(l_rotatedVector_cpy, l_rotatedVector, sizeof(float) * 4);
			RQuaternion_mul(l_rotatedVector_cpy, l_tmp, l_rotatedVector);
		}

		RVector_3_normalize(l_rotatedVector, p_out);
	};
}