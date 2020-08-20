#pragma once

#include "DataStructures/ARRAY_def.h"
#include "Objects/Resource/Mesh_def.h"

void MeshResourceProvider_Alloc(MESHRESOURCE_PROVIDER_PTR p_provider);
void MeshResourceProvider_Free(MESHRESOURCE_PROVIDER_PTR p_provider);
void MeshResourceProvider_UseResource(MESHRESOURCE_PROVIDER_PTR p_provider, char* p_meshSourceFilePath, MESHRESOURCE_HANDLE* out_mesh);
void MeshResourceProvider_ReleaseResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key);