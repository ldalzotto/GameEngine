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
		/*
		float l_qxx = quat[0] * quat[0];
		float l_qxy = quat[0] * quat[1];
		float l_qxz = quat[0] * quat[2];
		float l_qxw = quat[0] * quat[3];

		float l_qyy = quat[1] * quat[1];
		float l_qyz = quat[1] * quat[2];
		float l_qyw = quat[1] * quat[3];

		float l_qzz = quat[2] * quat[2];
		float l_qzw = quat[2] * quat[3];

		//RIGHT
		out_axis[0][0] = 1 - (2 * l_qyy) - (2 * l_qzz);
		out_axis[0][1] = (2 * l_qxy) + (2 * l_qzw);
		out_axis[0][2] = (2 * l_qxz) - (2 * l_qyw);
		RVector_3_normalize(out_axis[0], out_axis[0]);

		//UP
		out_axis[1][0] = (2 * l_qxy) - (2 * l_qzw);
		out_axis[1][1] = 1 - (2 * l_qxx) - (2 * l_qzz);
		out_axis[1][2] = (2 * l_qyz) + (2 * l_qxw);
		RVector_3_normalize(out_axis[1], out_axis[1]);

		//Forward
		out_axis[2][0] = (2 * l_qxz) + (2 * l_qyw);
		out_axis[2][1] = (2 * l_qyz) - (2 * l_qxw);
		out_axis[2][2] = 1 - (2 * l_qxx) - (2 * l_qyy);
		RVector_3_normalize(out_axis[2], out_axis[2]);
		*/
	};

	/*

	float l_qxx = p_quaternion->x * p_quaternion->x;
		float l_qxy = p_quaternion->x * p_quaternion->y;
		float l_qxz = p_quaternion->x * p_quaternion->z;
		float l_qxw = p_quaternion->x * p_quaternion->w;

		float l_qyy = p_quaternion->y * p_quaternion->y;
		float l_qyz = p_quaternion->y * p_quaternion->z;
		float l_qyw = p_quaternion->y * p_quaternion->w;

		float l_qzz = p_quaternion->z * p_quaternion->z;
		float l_qzw = p_quaternion->z * p_quaternion->w;

		//RIGHT
		out_right->x = 1 - (2 * l_qyy) - (2 * l_qzz);
		out_right->y = (2 * l_qxy) + (2 * l_qzw);
		out_right->z = (2 * l_qxz) - (2 * l_qyw);
		Vector3f_normalize(out_right);

		//UP
		out_up->x = (2 * l_qxy) - (2 * l_qzw);
		out_up->y = 1 - (2 * l_qxx) - (2 * l_qzz);
		out_up->z = (2 * l_qyz) + (2 * l_qxw);
		Vector3f_normalize(out_up);

		//Forward
		out_forward->x = (2 * l_qxz) + (2 * l_qyw);
		out_forward->y = (2 * l_qyz) - (2 * l_qxw);
		out_forward->z = 1 - (2 * l_qxx) - (2 * l_qyy);
		Vector3f_normalize(out_forward);*/
}