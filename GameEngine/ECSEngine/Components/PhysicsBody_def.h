#pragma once

#include "Component_def.h"
#include "World/Collider/BoxCollider_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef struct PhysicsBody_TYP
{
	ECS_ComponentHeader Header;
	BoxCollider_PTR Boxcollider;
}PhysicsBody, * PhysicsBody_PTR;
