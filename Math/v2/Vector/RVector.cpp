#include "RVector.h"

#include <string.h>
#include <stdlib.h>

#include "v2/Math.h"
#include "Functional/Equals/Equals.hpp"
#include "v2/Quaternion/RQuaternion.h"

namespace _MathV2
{
	/* Genrric methos */
	template <typename T>
	inline void RVector_min(const T* p_left, const T* p_right, T* p_out, short int p_size)
	{
		for (short int i = 0; i < p_size; i++)
		{
			p_out[i] = p_left[i] - p_right[i];
		}
	}

	template <typename T>
	inline T RVector_length(const T* p_vec, short int p_size)
	{
		T l_value = 0;
		for (short int i = 0; i < p_size; i++)
		{
			l_value += (p_vec[i] * p_vec[i]);
		}
		return Math_sqrt<T>(l_value);
	};

	template <typename T>
	inline void Rvector_normalize(const T* p_vec, int p_size, T* p_out)
	{
		T l_length = RVector_length(p_vec, p_size);
		for (short int i = 0; i < p_size; i++)
		{
			p_out[i] = (p_vec[i] / l_length);
		}
	};

	template <typename T>
	inline float RVector_distance(const T* p_start, const T* p_end, const short int p_size, T* tmp_vec)
	{
		RVector_min(p_start, p_end, tmp_vec, p_size);
		return RVector_length(tmp_vec, p_size);
	};

	/**/

	const float RIGHT_arr[3] = { 1.0f, 0.0f, 0.0f };
	const float UP_arr[3] = { 0.0f, 1.0f, 0.0f };
	const float FORWARD_arr[3] = { 0.0f, 0.0f, 1.0f };

	inline void RVector_mul(const float* p_left, const int p_left_nb, const float p_value, float* p_out)
	{
		for (int i = 0; i < p_left_nb; i++)
		{
			p_out[i] = p_left[i] * p_value;
		}
	};

	inline void RVector_mul(const float* p_left, const int p_left_nb, const float* p_right, float* p_out)
	{
		for (int i = 0; i < p_left_nb; i++)
		{
			p_out[i] = p_left[i] * p_right[i];
		}
	};

	bool RVector_2_equals(const double p_left[2], const double p_right[2])
	{
		return _Core::Equals_double(&p_left[0], &p_right[0])
			&& _Core::Equals_double(&p_left[1], &p_right[1]);
	};

	bool RVector_2_equals(const float p_left[2], const float p_right[2])
	{
		return _Core::Equals_float(&p_left[0], &p_right[0])
			&& _Core::Equals_float(&p_left[1], &p_right[1]);
	};

	bool RVector_3_equals(const float p_left[3], const float p_right[3])
	{
		return _Core::Equals_float(&p_left[0], &p_right[0])
			&& _Core::Equals_float(&p_left[1], &p_right[1])
			&& _Core::Equals_float(&p_left[2], &p_right[2]);
	};

	void RVector_3_add(const float p_left[3], const float p_right[3], float p_out[3])
	{
		p_out[0] = p_left[0] + p_right[0];
		p_out[1] = p_left[1] + p_right[1];
		p_out[2] = p_left[2] + p_right[2];
	};

	void RVector_min_specification(const int* p_left, const int* p_right, int* p_out, short p_size) { RVector_min<int>(p_left, p_right, p_out, p_size); };
	void RVector_min_specification(const float* p_left, const float* p_right, float* p_out, short p_size) { RVector_min<float>(p_left, p_right, p_out, p_size); };
	void RVector_min_specification(const double* p_left, const double* p_right, double* p_out, short p_size) { RVector_min<double>(p_left, p_right, p_out, p_size); };

	void RVector_2_mul(const float p_left[2], const float p_right, float p_out[2])
	{
		RVector_mul(p_left, 2, p_right, p_out);
	};

	void RVector_3_mul(const float p_left[3], const float p_right, float p_out[3])
	{
		RVector_mul(p_left, 3, p_right, p_out);
	};

	void RVector_3_mul(const float p_left[3], const float p_right[3], float p_out[3])
	{
		RVector_mul(p_left, 3, p_right, p_out);
	};

	void RVector_4_mul(const float p_left[4], const float p_right, float p_out[4])
	{
		RVector_mul(p_left, 4, p_right, p_out);
	};

	void RVector_3_inv(const float p_vec[3], float p_out[3])
	{
		p_out[0] = 1 / p_vec[0];
		p_out[1] = 1 / p_vec[1];
		p_out[2] = 1 / p_vec[2];
		// RVector_mul(p_vec, 3, RVector_3_dot(p_vec, p_vec), p_out);
	};

	float RVector_length_specialization(const int* p_vec, short int p_size) { return RVector_length<int>(p_vec, p_size); }
	float RVector_length_specialization(const float* p_vec, short int p_size) { return RVector_length<float>(p_vec, p_size); }
	double RVector_length_specialization(const double* p_vec, short int p_size) { return RVector_length<double>(p_vec, p_size); }

	float RVector_distance_specialization(const float* p_start, const float* p_end, const short int p_size, float* tmp_vec) { return RVector_distance<float>(p_start, p_end, p_size, tmp_vec); };
	float RVector_distance_specialization(const int* p_start, const int* p_end, const short int p_size, int* tmp_vec) { return RVector_distance<int>(p_start, p_end, p_size, tmp_vec); };

	void RVector_normalize_specialization(const float* p_vec, short int p_size, float* p_out) { Rvector_normalize(p_vec, p_size, p_out); };

	float RVector_3_dot(const float p_left[3], const float p_right[3])
	{
		return
			(p_left[0] * p_right[0]) +
			(p_left[1] * p_right[1]) +
			(p_left[2] * p_right[2]);
	};

	void RVector_3_cross(const float p_left[3], const float p_right[3], float p_out[3])
	{
		p_out[0] = (p_left[1] * p_right[2]) - (p_left[2] * p_right[1]);
		p_out[1] = (p_left[2] * p_right[0]) - (p_left[0] * p_right[2]);
		p_out[2] = (p_left[0] * p_right[1]) - (p_left[1] * p_right[0]);
	};

	void RVector_3_project(const float p_vec[3], const float p_projectedOn[3], float p_out[3])
	{
		RVector_3_mul(p_projectedOn, RVector_3_dot(p_vec, p_projectedOn) / RVector_length(p_projectedOn, 3), p_out);
	};

	float RVector_3_angle(const float p_begin[3], const float p_end[3])
	{
		return acosf(
			RVector_3_dot(p_begin, p_end) / (RVector_length(p_begin, 3) * RVector_length(p_end, 3))
		);
	};

	float RVector_3_angle_normalized(const float p_begin[3], const float p_end[3])
	{
		return acosf(
			RVector_3_dot(p_begin, p_end)
		);
	};

	short int RVector_3_angleSign(const float p_begin[3], const float p_end[3], const float p_referenceAxis[3])
	{
		float l_cross[3];
		RVector_3_cross(p_begin, p_end, l_cross);
		float l_dot = RVector_3_dot(l_cross, p_referenceAxis);
		return l_dot >= 0.0000f ? 1 : -1;
	};

	void RVector_3_rotate(const float p_vector[3], const float p_rotation[4], float p_out[3])
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

		Rvector_normalize(l_rotatedVector, 3, p_out);
	};
}