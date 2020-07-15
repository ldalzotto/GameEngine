#include "Vector.h"

#include "DataStructures/Specifications/String.hpp"
#include "Functional/ToString/ToString.hpp"

namespace _GameEngine::_Math
{
	Vector3f RIGHT{ 1.0f, 0.0f, 0.0f };
	Vector3f UP{ 0.0f, 1.0f, 0.0f };
	Vector3f FORWARD{ 0.0f, 0.0f, 1.0f };

	void Vector3f_toString(Vector3f* p_vector, _Core::String* p_string)
	{
		_Core::String_append(p_string, "[x : ");
		_Core::ToString_float(p_string, &p_vector->x);
		_Core::String_append(p_string, ", y : ");
		_Core::ToString_float(p_string, &p_vector->y);
		_Core::String_append(p_string, ", z : ");
		_Core::ToString_float(p_string, &p_vector->z);
		_Core::String_append(p_string, "]");
	}
}