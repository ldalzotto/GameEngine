#pragma once

#include "Math/Vector/Vector.h"

namespace _Core
{
	template<typename T>
	struct ArrayT;

	template<typename T>
	struct VectorT;
}

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

	/** Returned RayCastHits are not sorted. */
	void RayCastAll(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
	bool RayCast(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, RaycastHit* out_hit);
	void RayCastAll_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
	bool RayCast_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, RaycastHit* out_hit);
}