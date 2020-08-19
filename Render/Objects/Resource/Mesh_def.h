#pragma once

#include "Vertex_def.h"
#include "_interface/DataStructures/ARRAY_def.h"
#include "Objects/Resource/Polygon_def.h"

typedef size_t VertexIndex;

typedef struct POLYGON_VERTEXINDEX_TYP
{
	POLYGON_TYPE_DEFINITION(VertexIndex)
} POLYGON_VERTEXINDEX, * POLYGON_VERTEXINDEX_PTR;

typedef struct ARRAY_POLYGON_VERTEXINDEX_TYP
{
	ARRAY_TYPE_DEFINITION(POLYGON_VERTEXINDEX)
}ARRAY_POLYGON_VERTEXINDEX, * ARRAY_POLYGON_VERTEXINDEX_PTR;

typedef struct MESH_TYP
{
	ARRAY_VERTEX Vertices;
	ARRAY_POLYGON_VERTEXINDEX Polygons;
}MESH, *MESH_PTR;
