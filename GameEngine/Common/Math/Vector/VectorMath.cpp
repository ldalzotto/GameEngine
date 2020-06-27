#include "VectorMath.h"

#include <math.h>
#include "Math/Math.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
	void Vector3f_build(Vector4f* p_xyz, Vector3f* out)
	{
		out->x = p_xyz->x;
		out->y = p_xyz->y;
		out->z = p_xyz->z;
	};


	float Vector3f_length(Vector3f* p_vec)
	{
		return
			sqrtf((p_vec->x * p_vec->x) +
				(p_vec->y * p_vec->y) +
				(p_vec->z * p_vec->z));
	};

	float Vector3f_distance(Vector3f* p_vec, Vector3f* p_target)
	{
		Vector3f l_delta;
		Vector3f_min(p_target, p_vec, &l_delta);
		return Vector3f_length(&l_delta);
	};

	void Vector3f_normalize(Vector3f* p_vec)
	{
		float l_length = Vector3f_length(p_vec);
		if (l_length != 0)
		{
			p_vec->x = p_vec->x / l_length;
			p_vec->y = p_vec->y / l_length;
			p_vec->z = p_vec->z / l_length;
		}
	};

	bool Vector3f_equals(Vector3f* left, Vector3f* right)
	{
		return
			fabsf(left->x - right->x) < FLOAT_TOLERANCE &&
			fabsf(left->y - right->y) < FLOAT_TOLERANCE &&
			fabsf(left->z - right->z) < FLOAT_TOLERANCE
			;
	};

	void Vector3f_mul(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out)
	{
		p_out->x = p_vector->x * p_other->x;
		p_out->y = p_vector->y * p_other->y;
		p_out->z = p_vector->z * p_other->z;
	};

	void Vector3f_mul(Vector3f* p_vector, float p_value, Vector3f* p_out)
	{
		p_out->x = p_vector->x * p_value;
		p_out->y = p_vector->y * p_value;
		p_out->z = p_vector->z * p_value;
	};

	void Vector3f_add(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out)
	{
		p_out->x = p_vector->x + p_other->x;
		p_out->y = p_vector->y + p_other->y;
		p_out->z = p_vector->z + p_other->z;
	};

	void Vector3f_min(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out)
	{
		p_out->x = p_vector->x - p_other->x;
		p_out->y = p_vector->y - p_other->y;
		p_out->z = p_vector->z - p_other->z;
	};

	float Vector3f_dot(Vector3f* p_vector, Vector3f* p_other)
	{
		return
			(p_vector->x * p_other->x) +
			(p_vector->y * p_other->y) +
			(p_vector->z * p_other->z);
	};

	void Vector3f_cross(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out)
	{
		p_out->x = (p_vector->y * p_other->z) - (p_vector->z * p_other->y);
		p_out->y = (p_vector->z * p_other->x) - (p_vector->x * p_other->z);
		p_out->z = (p_vector->x * p_other->y) - (p_vector->y * p_other->x);
	};

	float Vector3f_angle(Vector3f* p_vector, Vector3f* p_other)
	{
		float p_vectorLength = Vector3f_length(p_vector);
		float p_otherLength = Vector3f_length(p_other);
		return acosf(
			Vector3f_dot(p_vector, p_other) / (p_vectorLength * p_otherLength)
		);
	};

	bool Vector4f_equals(Vector4f* left, Vector4f* right)
	{
		return
			fabsf(left->x - right->x) < FLOAT_TOLERANCE &&
			fabsf(left->y - right->y) < FLOAT_TOLERANCE &&
			fabsf(left->z - right->z) < FLOAT_TOLERANCE &&
			fabsf(left->w - right->w) < FLOAT_TOLERANCE
			;
	};

	void Vector4f_build(Vector3f* p_xyz, float p_w, Vector4f* out)
	{
		out->x = p_xyz->x;
		out->y = p_xyz->y;
		out->z = p_xyz->z;
		out->w = p_w;
	};

	void Vector4f_build(Vector2f* p_xy, float p_z, float p_w, Vector4f* out)
	{
		out->x = p_xy->x;
		out->y = p_xy->y;
		out->z = p_w;
		out->w = p_w;
	};

	void Vector4f_mul(Vector4f* p_vect, float p_value, Vector4f* p_out)
	{
		p_out->x = p_vect->x * p_value;
		p_out->y = p_vect->y * p_value;
		p_out->z = p_vect->z * p_value;
		p_out->w = p_vect->w * p_value;
	};

	float Vector4f_length(Vector4f* p_vec)
	{
		return
			sqrtf((p_vec->x * p_vec->x) +
				(p_vec->y * p_vec->y) +
				(p_vec->z * p_vec->z) +
				(p_vec->w * p_vec->w));
	};

	void Vector4f_add(Vector4f* p_vector, Vector4f* p_other, Vector4f* p_out)
	{
		p_out->x = p_vector->x + p_other->x;
		p_out->y = p_vector->y + p_other->y;
		p_out->z = p_vector->z + p_other->z;
		p_out->w = p_vector->w + p_other->w;
	};

	void Vector4f_add(Vector4f* p_vector, Vector3f* p_other, Vector4f* p_out)
	{
		p_out->x = p_vector->x + p_other->x;
		p_out->y = p_vector->y + p_other->y;
		p_out->z = p_vector->z + p_other->z;
		p_out->w = p_vector->w;
	};
}