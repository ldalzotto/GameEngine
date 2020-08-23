#pragma once

extern "C"
{
#include "Component_def.h"
}
namespace _GameEngine::_Physics
{
	struct BoxCollider;
}

typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef struct PhysicsBody_TYP
{
	ECS_ComponentHeader Header;
	_GameEngine::_Physics::BoxCollider* Boxcollider;
}PhysicsBody, * PhysicsBody_PTR;
