#pragma once

#include "DataStructures/ARRAY_def.h"
#include "Objects/Resource/Mesh_def.h"

typedef size_t MESHRESOURCE_KEY;

typedef struct MESHRESOURCE_TYP
{
	MESHRESOURCE_KEY Key;
	MESH Mesh;
	size_t UsageCount;
}MESHRESOURCE, * MESHRESOURCE_PTR;

typedef struct MESHRESOURCE_PROVIDER_TYP
{
	ARRAY_TYPE_DEFINITION(MESHRESOURCE)
}MESHRESOURCE_PROVIDER, * MESHRESOURCE_PROVIDER_PTR;

void MeshResourceProvider_Alloc(MESHRESOURCE_PROVIDER_PTR p_provider);
void MeshResourceProvider_Free(MESHRESOURCE_PROVIDER_PTR p_provider);
void MeshResourceProvider_UseResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key, MESHRESOURCE_PTR* out_mesh);
void MeshResourceProvider_ReleaseResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key);