#pragma once

#include "Vertex_def.h"
#include "DataStructures/ARRAY_def.h"
#include "Objects/Resource/Polygon_def.h"

typedef size_t VertexIndex;

typedef struct Polygon_VertexIndex_TYP
{
	POLYGON_TYPE_DEFINITION(VertexIndex)
} Polygon_VertexIndex, * Polygon_VertexIndex_PTR;

typedef struct Array_Polygon_VertexIndex_TYP
{
	ARRAY_TYPE_DEFINITION(Polygon_VertexIndex)
}Array_Polygon_VertexIndex, * Array_Polygon_VertexIndex_PTR;

typedef struct Mesh_TYP
{
	Array_Vertex Vertices;
	Array_Polygon_VertexIndex Polygons;
}Mesh, *Mesh_PTR;

typedef size_t MeshResource_KEY;

typedef struct MeshResource_TYP
{
	MeshResource_KEY Key;
	Mesh Mesh;
	size_t UsageCount;
}MeshResource, * MeshResource_HANDLE;

typedef struct MeshResource_Provider_TYP
{
	ARRAY_TYPE_DEFINITION(MeshResource_HANDLE)
}MeshResource_Provider, * MeshResource_Provider_PTR;