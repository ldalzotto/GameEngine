#pragma once

#include "v2/_interface/VectorC_def.h"
#include "World/Collider/BoxCollider_def.h"
#include "World_def.h"
#include "DataStructures/ARRAY.h"

typedef struct RaycastHit_TYP
{
	BoxCollider_PTR Collider;
	Vector3f HitPoint;
}RaycastHit, * RaycastHit_PTR;

typedef struct Array_RaycastHit_TYP
{
	ARRAY_TYPE_DEFINITION(RaycastHit)
}Array_RaycastHit, * Array_RaycastHit_PTR;

ARRAY_ALLOC_FUNCTION(RaycastHit, Array_RaycastHit_PTR, RaycastHit)
ARRAY_FREE_FUNCTION(RaycastHit, Array_RaycastHit_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(RaycastHit, Array_RaycastHit_PTR, RaycastHit)

/** Returned RayCastHits are not sorted. */
void RayCastAll_Realloc(World* p_world, Vector3f_PTR p_begin, Vector3f_PTR p_end, Array_RaycastHit_PTR out_intersectionPoints);
bool RayCast(World* p_world, Vector3f_PTR p_begin, Vector3f_PTR p_end, RaycastHit* out_hit);
void RayCastAll_Against_Realloc(Array_BoxColliderPTR_PTR p_comparedColliders, Vector3f_PTR p_begin, Vector3f_PTR p_end, Array_RaycastHit_PTR out_intersectionPoints);
bool RayCast_Against(Array_BoxColliderPTR_PTR p_comparedColliders, Vector3f_PTR p_begin, Vector3f_PTR p_end, RaycastHit* out_hit);
