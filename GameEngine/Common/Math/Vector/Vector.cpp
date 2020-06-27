#include "Vector.h"

#include "DataStructures/String.h"
#include "DataStructures/StringConverterPrimitives.h"

namespace _GameEngine::_Math
{
	Vector3f RIGHT{ 1.0f, 0.0f, 0.0f };
	Vector3f UP{ 0.0f, 1.0f, 0.0f };
	Vector3f FORWARD{ 0.0f, 0.0f, 1.0f };

	void Vector3f_toString(Vector3f* p_vector, _Core::String* p_string)
	{
		p_string->reserve(50);
		p_string->append("[x : ");
		_Core::String_appendFloat(p_string, p_vector->x);
		p_string->append(", y : ");
		_Core::String_appendFloat(p_string, p_vector->y);
		p_string->append(", z : ");
		_Core::String_appendFloat(p_string, p_vector->z);
		p_string->append("]");
	}
}