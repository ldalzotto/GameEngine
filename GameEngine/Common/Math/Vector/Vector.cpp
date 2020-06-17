#include "Vector.h"

#include <math.h>

namespace _GameEngine::_Math
{
	Vector3f RIGHT{ 1.0f, 0.0f, 0.0f };
	Vector3f UP{ 0.0f, 1.0f, 0.0f };
	Vector3f FORWARD{ 0.0f, 0.0f, 1.0f };

	void Vector3f_fromGLM(glm::vec3* p_vec, Vector3f* p_out)
	{
		p_out->x = p_vec->x;
		p_out->y = p_vec->y;
		p_out->z = p_vec->z;
	};

	void Vector3f_toGLM(Vector3f* p_vec, glm::vec3* p_out)
	{
		p_out->x = p_vec->x;
		p_out->y = p_vec->y;
		p_out->z = p_vec->z;
	};

	float Vector3f_length(Vector3f* p_vec)
	{
		return
			sqrt((p_vec->x * p_vec->x) +
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

	bool Vector3f_equals(Vector3f* left, Vector3f* right)
	{
		return
			left->x == right->x &&
			left->y == right->y &&
			left->z == right->z
			;
	};

	void Vector3f_build(Vector4f* p_xyz, Vector3f* out)
	{
		out->x = p_xyz->x;
		out->y = p_xyz->y;
		out->z = p_xyz->z;
	};

	void Vector4f_build(Vector3f* p_xyz, float p_w, Vector4f* out)
	{
		out->x = p_xyz->x;
		out->y = p_xyz->y;
		out->z = p_xyz->z;
		out->w = p_w;
	};
}