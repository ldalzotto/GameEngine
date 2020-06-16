#include "VectorTransform.h"

#include <math.h>
#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
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
		return acos(
			Vector3f_dot(p_vector, p_other) / (p_vectorLength * p_otherLength)
		);
	};
}