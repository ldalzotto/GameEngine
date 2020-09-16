#pragma once

#include "DataStructures/ARRAY_def.h"
#include "Objects/Resource/Mesh_def.h"
#include "Asset/AssetPaths.h"

void MeshResourceProvider_Alloc(MeshResource_Provider_PTR p_provider);
void MeshResourceProvider_Free(MeshResource_Provider_PTR p_provider);
void MeshResourceProvider_UseResource(MeshResource_Provider_PTR p_provider, Assetpath_PTR p_meshSourceFilePath, MeshResource_HANDLE* out_mesh);
void MeshResourceProvider_ReleaseResource(MeshResource_Provider_PTR p_provider, MeshResource_KEY* p_key);