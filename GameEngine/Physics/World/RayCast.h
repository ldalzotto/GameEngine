#pragma once

#include "DataStructures/VectorT.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine
{
	namespace _Physics { struct World; struct BoxCollider; }
}

namespace _GameEngine::_Physics
{
	struct RaycastHit
	{
		BoxCollider* Collider;
		_Math::Vector3f HitPoint;
	};

	void RayCastAll(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
}