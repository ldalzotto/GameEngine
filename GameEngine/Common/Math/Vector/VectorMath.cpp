#include "VectorMath.h"

#include "Functional/Equals/Equals.hpp"
#include <math.h>
#include "Math/Math.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
	double Vector2d_length(Vector2d* p_vector)
	{
		return
			sqrtf((p_vector->x * p_vector->x) +
				(p_vector->y * p_vector->y));
	};

	float Vector3f_length(Vector3f* p_vec)
	{
		return
			sqrtf((p_vec->x * p_vec->x) +
				(p_vec->y * p_vec->y) +
				(p_vec->z * p_vec->z));
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

	void Vector3f_project(Vector3f* p_vector, Vector3f* p_projectedOn, Vector3f* p_out)
	{
		_Math::Vector3f_mul(p_projectedOn, _Math::Vector3f_dot(p_vector, p_projectedOn) / _Math::Vector3f_length(p_projectedOn), p_out);
	};

	float Vector3f_angle_signed(Vector3f* p_p1, Vector3f* p_p2)
	{
		return acosf(
			Vector3f_dot(p_p1, p_p2) / (Vector3f_length(p_p1) * Vector3f_length(p_p2))
		);
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
			_Core::Equals_float(&left->x, &right->x) &&
			_Core::Equals_float(&left->y, &right->y) &&
			_Core::Equals_float(&left->z, &right->z) &&
			_Core::Equals_float(&left->w, &right->w);
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

	void Vector4f_homogenize_w(Vector4f* p_vect, Vector4f* p_out)
	{
		p_out->x = (p_vect->x / p_vect->w);
		p_out->y = (p_vect->y / p_vect->w);
		p_out->z = (p_vect->z / p_vect->w);
		p_out->w = (p_vect->w / p_vect->w);
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