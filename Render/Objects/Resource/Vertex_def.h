#pragma once

#include "v2/_interface/VectorC_def.h"
#include "_interface/DataStructures/ARRAY_def.h"

typedef struct VERTEX_TYP
{
	VECTOR3F LocalPosition;
}VERTEX, * VERTEX_PTR;

typedef struct ARRAY_VERTEX_TYP
{
	ARRAY_TYPE_DEFINITION(VERTEX)
} ARRAY_VERTEX, * ARRAY_VERTEX_PTR;