#pragma once

#include "Vertex_def.h"
#include "DataStructures/ARRAY_def.h"
#include "Objects/Resource/Polygon_def.h"

typedef size_t VertexIndex;

/************** VERTEX INDEX *****************/
#if 1
typedef struct Polygon_VertexIndex_TYP
{
	POLYGON_TYPE_DEFINITION(VertexIndex)
} Polygon_VertexIndex, * Polygon_VertexIndex_PTR;

typedef struct Polygon_VertexIndex_HANDLE_TYP
{
	size_t Handle;
}Polygon_VertexIndex_HANDLE, * Polygon_VertexIndex_HANDLE_PTR;

typedef struct Array_Polygon_VertexIndex_HANDLE_TYP
{
	ARRAY_TYPE_DEFINITION(Polygon_VertexIndex_HANDLE)
}Array_Polygon_VertexIndex_HANDLE, * Array_Polygon_VertexIndex_HANDLE_PTR;

typedef struct Array_Polygon_VertexIndex_TYP
{
	ARRAY_TYPE_DEFINITION(Polygon_VertexIndex)
}Array_Polygon_VertexIndex, * Array_Polygon_VertexIndex_PTR;
#endif

/************** UV *****************/ 
#if 1
typedef Vector2f UV, *UV_PTR;
typedef Polygon2f Polygon_UV, *Polygon_UV_PTR;
typedef struct Polgyon_UV_HANDLE_TYP
{
	size_t Handle;
}Polgyon_UV_HANDLE, * Polgyon_UV_HANDLE_PTR;

typedef struct Array_PolygonUV_TYP
{
	ARRAY_TYPE_DEFINITION(Polygon_UV)
}Array_PolygonUV, * Array_PolygonUV_PTR;

typedef struct Array_PolygonUVHandle_TYP
{
	ARRAY_TYPE_DEFINITION(Polgyon_UV_HANDLE)
}Array_PolygonUVHandle, * Array_PolygonUVHandle_PTR;
#endif

typedef struct MeshPerVertexData_TYP
{
	Array_PolygonUVHandle UV1;
}MeshPerVertexData, *MeshPerVertexData_PTR;

typedef struct Mesh_TYP
{
	Array_VertexHANDLE Vertices;
	Array_Polygon_VertexIndex_HANDLE Polygons;
	MeshPerVertexData PerVertexData;
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