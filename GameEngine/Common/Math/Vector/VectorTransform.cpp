#include "VectorTransform.h"

#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
	void Vector3f_mul(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out)
	{
		p_out->x = p_vector->x * p_other->x;
		p_out->y = p_vector->y * p_other->y;
		p_out->z = p_vector->z * p_other->z;
	};

	void Vector3f_mul(Vector3f* p_vector, float p_value)
	{
		p_vector->x = p_vector->x * p_value;
		p_vector->y = p_vector->y * p_value;
		p_vector->z = p_vector->z * p_value;
	};
}