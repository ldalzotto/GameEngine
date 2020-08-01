#pragma once

#include "ECS/ComponentT.hpp"
#include "Math/Box/Box.hpp"

namespace _GameEngine::_Physics
{
	struct BoxCollider;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererBoundType;

	struct MeshRendererBound
	{
		_ECS::ComponentHeaderT<MeshRendererBound> ComponentHeader;
		_Math::Box BoundingBox;
		_Physics::BoxCollider* Boxcollider;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<MeshRendererBound>()
	{
		return &MeshRendererBoundType;
	};
}