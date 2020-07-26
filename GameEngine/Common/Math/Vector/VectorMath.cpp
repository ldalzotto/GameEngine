#include "VectorMath.h"

#include "Functional/Equals/Equals.hpp"
#include <math.h>
#include "Math/Math.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
	float Vector2f_length(Vector2f* p_vector)
	{
		return
			sqrtf((p_vector->x * p_vector->x) +
				(p_vector->y * p_vector->y));
	};
	
	void Vector2f_normalize(Vector2f* p_vector)
	{
		float l_length = Vector2f_length(p_vector);
		if (l_length != 0)
		{
			p_vector->x = p_vector->x / l_length;
			p_vector->y = p_vector->y / l_length;
		}
	};

	
	float Vector2f_dot(Vector2f* p_left, Vector2f* p_right)
	{
		return (
				(p_left->x * p_right->x) +
				(p_left->y * p_right->y)
			);
	};

	void Vector2d_min(Vector2d* p_left, Vector2d* p_right, Vector2d* p_out)
	{
		p_out->x = p_left->x - p_right->x;
		p_out->y = p_left->y - p_right->y;
	};

	double Vector2d_length(Vector2d* p_vector)
	{
		return
			sqrtf((p_vector->x * p_vector->x) +
				(p_vector->y * p_vector->y));
	};

	void Vector2d_normalize(Vector2d* p_vector)
	{
		double l_length = Vector2d_length(p_vector);
		if (l_length != 0)
		{
			p_vector->x = p_vector->x / l_length;
			p_vector->y = p_vector->y / l_length;
		}
	};

	bool Vector2d_equals(Vector2d* p_left, Vector2d* p_right)
	{
		return
			_Core::Equals_double(&p_left->x, &p_right->x) &&
			_Core::Equals_double(&p_left->y, &p_right->y);
	};

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
			_Core::Equals_float(&left->x, &right->x) &&
			_Core::Equals_float(&left->y, &right->y) &&
			_Core::Equals_float(&left->z, &right->z)
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

	void Vector3f_project(Vector3f* p_vector, Vector3f* p_projectedOn, Vector3f* p_out)
	{
		_Math::Vector3f_mul(p_projectedOn, _Math::Vector3f_dot(p_vector, p_projectedOn) / _Math::Vector3f_length(p_projectedOn), p_out);
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