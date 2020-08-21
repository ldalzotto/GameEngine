#pragma once

extern "C"
{
#include "v2/_interface/VectorC_def.h"
}

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
		Vector3f HitPoint;
	};

	/** Returned RayCastHits are not sorted. */
	void RayCastAll(World* p_world, Vector3f_PTR p_begin, Vector3f_PTR p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
	bool RayCast(World* p_world, Vector3f_PTR p_begin, Vector3f_PTR p_end, RaycastHit* out_hit);
	void RayCastAll_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, Vector3f_PTR p_begin, Vector3f_PTR p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
	bool RayCast_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, Vector3f_PTR p_begin, Vector3f_PTR p_end, RaycastHit* out_hit);
}