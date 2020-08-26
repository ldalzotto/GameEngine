#pragma once

#include "TransformComponent_def.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/VectorC_def.h"
#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"

typedef struct TransformInitInfo_TYP
{
	Vector3f LocalPosition;
	Quaternion4f LocalRotation;
	Vector3f LocalScale;
}TransformInitInfo, * TransformInitInfo_PTR;

extern const TransformInitInfo TransformInitInfo_Default;

ECS_ALLOCCOMPONENT_FUNCTION(TransformComponent, TRANSFORM_COMPONENT_TYPE, TransformComponent, TransformComponent_PTR);
ECS_ADDCOMPONENT_FUNCTION(TransformComponent, TransformComponent_PTR);
ECS_GETCOMPONENT_FUNCTION(TransformComponent, TRANSFORM_COMPONENT_TYPE, TransformComponent_PTR);

void TransformComponent_init(TransformComponent* p_component, TransformInitInfo* p_transformInitInfo);
void TransformComponent_Free(TransformComponent* p_transformComponent, ECS* p_ecs);
TransformComponent* TransformComponent_castFromTransform(Transform_PTR p_transform);