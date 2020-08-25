#pragma once

#include "PhysicsBody_def.h"

#include "ECS.h"
#include "Types_def.h"

ECS_ALLOCCOMPONENT_FUNCTION(PhysicsBody, PHYSICSBODY_COMPONENT_TYPE, PhysicsBody, PhysicsBody_PTR);
ECS_GETCOMPONENT_FUNCTION(PhysicsBody, PHYSICSBODY_COMPONENT_TYPE, PhysicsBody_PTR);