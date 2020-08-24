#pragma once

#include "BoxCollider_def.h"
#include "DataStructures/ARRAY.h"

BoxCollider* BoxCollider_alloc(BoxF_PTR p_box, Transform_PTR ptransform);
void BoxCollider_free(BoxCollider** p_boxCollider);

ARRAY_ALLOC_FUNCTION(BoxColliderPTR, Array_BoxColliderPTR_PTR, BoxCollider_PTR)
ARRAY_FREE_FUNCTION(BoxColliderPTR, Array_BoxColliderPTR_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_VALUE(BoxColliderPTR, Array_BoxColliderPTR_PTR, BoxCollider_PTR)
ARRAY_ERASE_FUNCTION(BoxColliderPTR, Array_BoxColliderPTR_PTR, BoxCollider_PTR)