#pragma once

#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "Objects/Resource/Mesh_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct RENDEREDOBJECT_TYP
{
	MESH_PTR Mesh;
	BOXF_PTR MeshBoundingBox;
	MATRIX4F ModelMatrix;
}RENDEREDOBJECT, * RENDEREDOBJECT_PTR, * RENDEREDOBJECT_HANDLE, ** RENDEREDOBJECT_HANDLE_PTR;

typedef struct ARRAY_REDEREDOBJECT_HANDLE_TYP
{
	ARRAY_TYPE_DEFINITION(RENDEREDOBJECT_HANDLE)
}ARRAY_REDEREDOBJECT_HANDLE, * ARRAY_REDEREDOBJECT_HANDLE_PTR;