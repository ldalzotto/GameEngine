#pragma once

#include "MeshResourceProviderV2.h"
#include "DataStructures/ARRAY.h"
#include "File/ObjReader.h"
#include "Error/ErrorHandler.h"
#include "Functional/Hash.h"
#include "Mesh.h"

inline void Arr_Alloc_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider) { Arr_Alloc((ARRAY_PTR)p_resourceProvider, sizeof(MESHRESOURCE), 0); };
inline void Arr_Free_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider) { Arr_Free((ARRAY_PTR)p_resourceProvider); };
inline void Arr_PushBackRealloc_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider, MESHRESOURCE_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((ARRAY_PTR)p_resourceProvider, sizeof(MESHRESOURCE), (char*)p_value)); };
inline void Arr_Erase_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider, size_t p_index) { HANDLE_ERR(Arr_Erase((ARRAY_PTR)p_resourceProvider, sizeof(MESHRESOURCE), p_index)); };

void MeshResourceProvider_Alloc(MESHRESOURCE_PROVIDER_PTR p_provider)
{
	Arr_Alloc_MeshResourceProvider(p_provider);
};

void MeshResourceProvider_Free(MESHRESOURCE_PROVIDER_PTR p_provider)
{
	Arr_Free_MeshResourceProvider(p_provider);
};

size_t MeshResourceProvider_GetResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key, MESHRESOURCE_PTR* out_mesh)
{
	for (size_t i = 0; i < p_provider->Size; i++)
	{
		MESHRESOURCE_PTR l_mesResource = &p_provider->Memory[i];
		if (l_mesResource->Key == *p_key)
		{
			*out_mesh = l_mesResource;
			return i;
		}
	}
	*out_mesh = NULL;
	return -1;
};

void MeshResourceProvider_UseResource(MESHRESOURCE_PROVIDER_PTR p_provider, char* p_meshSourceFilePath, MESHRESOURCE_PTR* out_mesh)
{
	MESHRESOURCE_KEY l_key = HashCombine_string(0, p_meshSourceFilePath);
	MeshResourceProvider_GetResource(p_provider, &l_key, out_mesh);
	if (!*out_mesh)
	{
		MESHRESOURCE l_insertedResource = {0};
		Arr_PushBackRealloc_MeshResourceProvider(p_provider, &l_insertedResource);
		*out_mesh = &p_provider->Memory[p_provider->Size - 1];
		(*out_mesh)->Key = l_key;
		// Mesh Creation
		// *out_mesh = XXXX
		ObjReader_loadMesh(p_meshSourceFilePath, &(*out_mesh)->Mesh);
	}
	(*out_mesh)->UsageCount += 1;
};

void MeshResourceProvider_ReleaseResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key)
{
	MESHRESOURCE_PTR l_meshResource;
	size_t l_resourceIndex = MeshResourceProvider_GetResource(p_provider, p_key, &l_meshResource);
	if (l_meshResource)
	{
		(*l_meshResource).UsageCount -= 1;
		if ((*l_meshResource).UsageCount == 0)
		{ 
			Mesh_Free(&l_meshResource->Mesh);
			Arr_Erase_MeshResourceProvider(p_provider, l_resourceIndex);
		}
	}
};