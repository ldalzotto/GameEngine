#pragma once

namespace _Core
{
	struct String;
}

namespace _GameEngine::_Math
{
	struct Vector2f
	{
		float x;
		float y;
	};

	struct Vector2d
	{
		double x;
		double y;
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

	void Vector3f_toString(Vector3f* p_vector, _Core::String* p_string);

	struct Vector4f
	{
		float x;
		float y;
		float z;
		float w;
	};


}