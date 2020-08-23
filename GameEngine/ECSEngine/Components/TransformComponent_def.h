#pragma once

#include "v2/_interface/TransformC_def.h"
#include "Component_def.h"

typedef struct TransformComponent_TYP
{
	ECS_ComponentHeader Header;
	Transform Transform;
}TransformComponent, * TransformComponent_PTR;