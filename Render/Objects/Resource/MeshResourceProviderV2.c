#pragma once

#include "MeshResourceProviderV2.h"
#include "DataStructures/ARRAY.h"
#include "File/ObjReader.h"
#include "Error/ErrorHandler.h"

inline void Arr_Alloc_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider) { Arr_Alloc((ARRAY_PTR)p_resourceProvider, sizeof(MESHRESOURCE), 0); };
inline void Arr_Free_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider) { Arr_Free((ARRAY_PTR)p_resourceProvider); };
inline void Arr_PushBackRealloc_MeshResourceProvider(MESHRESOURCE_PROVIDER_PTR p_resourceProvider, MESHRESOURCE_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((ARRAY_PTR)p_resourceProvider, sizeof(MESHRESOURCE), (char*)p_value)); };

void MeshResourceProvider_Alloc(MESHRESOURCE_PROVIDER_PTR p_provider)
{
	Arr_Alloc_MeshResourceProvider(p_provider);
};

void MeshResourceProvider_Free(MESHRESOURCE_PROVIDER_PTR p_provider)
{
	Arr_Free_MeshResourceProvider(p_provider);
};

void MeshResourceProvider_GetResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key, MESHRESOURCE_PTR* out_mesh)
{
	for (size_t i = 0; i < p_provider->Size; i++)
	{
		MESHRESOURCE_PTR l_mesResource = &p_provider->Memory[i];
		if (l_mesResource->Key == *p_key)
		{
			*out_mesh = l_mesResource;
			return;
		}
	}
	*out_mesh = NULL;
};

void MeshResourceProvider_UseResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key, MESHRESOURCE_PTR* out_mesh)
{
	MeshResourceProvider_GetResource(p_provider, p_key, out_mesh);
	if (!*out_mesh)
	{
		MESHRESOURCE l_insertedResource = {0};
		Arr_PushBackRealloc_MeshResourceProvider(p_provider, &l_insertedResource);
		*out_mesh = &p_provider->Memory[p_provider->Size - 1];
		// Mesh Creation
		// *out_mesh = XXXX
		// ObjReader_loadMesh("ef", &(*out_mesh)->Mesh);
	}
	(*out_mesh)->UsageCount += 1;
};

void MeshResourceProvider_ReleaseResource(MESHRESOURCE_PROVIDER_PTR p_provider, MESHRESOURCE_KEY* p_key)
{
	MESHRESOURCE_PTR l_meshResource;
	MeshResourceProvider_GetResource(p_provider, p_key, &l_meshResource);
	if (l_meshResource)
	{
		(*l_meshResource).UsageCount -= 1;
		if ((*l_meshResource).UsageCount == 0)
		{ 
			//Free Mesh
		}
	}
};