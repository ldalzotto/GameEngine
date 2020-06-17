#pragma once

namespace _GameEngine::_Math
{
	struct Vector2f
	{
		float x;
		float y;
	};

	struct Vector3f
	{
		float x;
		float y;
		float z;
	};

	extern Vector3f RIGHT;
	extern Vector3f UP;
	extern Vector3f FORWARD;

	bool Vector3f_equals(Vector3f* left, Vector3f* right);
	void Vector3f_normalize(Vector3f* p_vec);
	float Vector3f_length(Vector3f* p_vec);

	struct Vector4f
	{
		float x;
		float y;
		float z;
		float w;
	};

	void Vector3f_build(Vector4f* p_xyz, Vector3f* out);
	void Vector4f_build(Vector3f* p_xyz, float p_w, Vector4f* out);

}