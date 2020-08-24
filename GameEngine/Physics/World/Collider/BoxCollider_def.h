#pragma once

#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/TransformC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct BoxCollider_TYP
{
	Transform_PTR Transform;
	BoxF_PTR Box;
}BoxCollider, * BoxCollider_PTR;

typedef struct Array_BoxColliderPTR_TYP
{
	ARRAY_TYPE_DEFINITION(BoxCollider_PTR)
}Array_BoxColliderPTR, * Array_BoxColliderPTR_PTR;
