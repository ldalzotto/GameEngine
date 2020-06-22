#pragma once

#include "ECS/Component.h"
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
}