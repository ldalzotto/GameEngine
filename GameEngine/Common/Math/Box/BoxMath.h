#pragma once

#include "DataStructures/VectorT.h"

namespace _GameEngine::_Math
{
	struct Box;
	struct Vector3f;
}

namespace _GameEngine::_Math
{
	void Box_build(Box* p_box, _Core::VectorT<_Math::Vector3f>* p_points);
	// void Box_grow(Box* p_box, Vector3f* p_growingPoint);
}