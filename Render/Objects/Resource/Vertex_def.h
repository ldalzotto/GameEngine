#pragma once

#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Vertex_TYP
{
	Vector3f LocalPosition;
}Vertex, * Vertex_PTR;

typedef struct Array_Vertex_TYP
{
	ARRAY_TYPE_DEFINITION(Vertex)
} Array_Vertex, * Array_Vertex_PTR;