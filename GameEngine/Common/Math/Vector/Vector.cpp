#include "Vector.h"

extern "C"
{
#include "Include/CoreV2.h"
}

namespace _GameEngine::_Math
{
	Vector3f RIGHT{ 1.0f, 0.0f, 0.0f };
	Vector3f UP{ 0.0f, 1.0f, 0.0f };
	Vector3f FORWARD{ 0.0f, 0.0f, 1.0f };

	void Vector3f_toString(Vector3f* p_vector, Core_String* p_string)
	{
		Core_string_append(p_string, "[x : ");
		Core_toString_float(p_string, &p_vector->x);
		Core_string_append(p_string, ", y : ");
		Core_toString_float(p_string, &p_vector->y);
		Core_string_append(p_string, ", z : ");
		Core_toString_float(p_string, &p_vector->z);
		Core_string_append(p_string, "]");
	}
}