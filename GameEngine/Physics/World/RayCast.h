#pragma once

#include "DataStructures/VectorT.h"

namespace _GameEngine
{
	namespace _Physics { struct World; struct BoxCollider; }
	namespace _Math { struct Vector3f; }
}

namespace _GameEngine::_Physics
{
	void RayCastAll(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<_Math::Vector3f>* out_intersectionPoints);
}