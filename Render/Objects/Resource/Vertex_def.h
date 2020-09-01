#pragma once

#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Vertex_TYP
{
	Vector4f LocalPosition;
}Vertex, * Vertex_PTR;

typedef struct Array_Vertex_TYP
{
	ARRAY_TYPE_DEFINITION(Vertex)
} Array_Vertex, * Array_Vertex_PTR;

typedef struct Vertex_HANDLE_TYP
{
	size_t Handle;
}Vertex_HANDLE, * Vertex_HANDLE_PTR;

typedef struct Array_VertexHANDLE_TYP
{
	ARRAY_TYPE_DEFINITION(Vertex_HANDLE)
}Array_VertexHANDLE, * Array_VertexHANDLE_PTR;
