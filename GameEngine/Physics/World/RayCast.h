#pragma once


#include "v2/Vector/Vector.hpp"

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
		_MathV2::Vector3<float> HitPoint;
	};

	/** Returned RayCastHits are not sorted. */
	void RayCastAll(World* p_world, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
	bool RayCast(World* p_world, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, RaycastHit* out_hit);
	void RayCastAll_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints);
	bool RayCast_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, RaycastHit* out_hit);
}