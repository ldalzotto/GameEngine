#pragma once

#include "ECS/ComponentT.hpp"
#include "Math/Box/Box.h"

namespace _GameEngine::_Physics
{
	struct BoxCollider;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererBoundType;

	struct MeshRendererBound
	{
		_Math::Box BoundingBox;
		_Physics::BoxCollider* Boxcollider;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<MeshRendererBound>()
	{
		return &MeshRendererBoundType;
	};
}