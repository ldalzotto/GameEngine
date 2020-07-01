#pragma once

#include "ECS/Component.h"
#include "Math/Box/Box.h"

namespace _GameEngine::_Physics
{
	struct BoxCollider;
}

namespace _GameEngine::_ECS
{
	struct MeshRendererBound
	{
		_Math::Box BoundingBox;
		_Physics::BoxCollider* Boxcollider;
	};

	template<>
	extern inline ComponentType* extractComponentType<MeshRendererBound>();

}