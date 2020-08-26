#pragma once

#include "MeshResourceProviderV2.h"
#include "DataStructures/ARRAY.h"
#include "File/ObjReader.h"
#include "Error/ErrorHandler.h"
#include "Functional/Hash.h"
#include "Mesh.h"

inline void Arr_Alloc_MeshResourceProvider(MeshResource_Provider_PTR p_resourceProvider) { Arr_Alloc(&p_resourceProvider->array, sizeof(MeshResource_HANDLE), 0); };
inline void Arr_Free_MeshResourceProvider(MeshResource_Provider_PTR p_resourceProvider) { Arr_Free(&p_resourceProvider->array); };
inline void Arr_PushBackRealloc_MeshResourceProvider(MeshResource_Provider_PTR p_resourceProvider, MeshResource_HANDLE* p_value) { HANDLE_ERR(Arr_PushBackRealloc(&p_resourceProvider->array, sizeof(MeshResource_HANDLE), (char*)p_value)); };
inline void Arr_Erase_MeshResourceProvider(MeshResource_Provider_PTR p_resourceProvider, size_t p_index) { HANDLE_ERR(Arr_Erase(&p_resourceProvider->array, sizeof(MeshResource_HANDLE), p_index)); };

void MeshResourceProvider_Alloc(MeshResource_Provider_PTR p_provider)
{
	Arr_Alloc_MeshResourceProvider(p_provider);
};

void MeshResourceProvider_Free(MeshResource_Provider_PTR p_provider)
{
	Arr_Free_MeshResourceProvider(p_provider);
};

size_t MeshResourceProvider_GetResource(MeshResource_Provider_PTR p_provider, MeshResource_KEY* p_key, MeshResource_HANDLE* out_mesh)
{
	for (size_t i = 0; i < p_provider->Size; i++)
	{
		MeshResource_HANDLE l_mesResource = p_provider->Memory[i];
		if (l_mesResource->Key == *p_key)
		{
			*out_mesh = l_mesResource;
			return i;
		}
	}
	*out_mesh = NULL;
	return -1;
};

void MeshResourceProvider_UseResource(MeshResource_Provider_PTR p_provider, char* p_meshSourceFilePath, MeshResource_HANDLE* out_mesh)
{
	MeshResource_KEY l_key = HashCombine_string(0, p_meshSourceFilePath);
	MeshResourceProvider_GetResource(p_provider, &l_key, out_mesh);
	if (!*out_mesh)
	{
		MeshResource_HANDLE l_instanciatedResource = (MeshResource_HANDLE) calloc(1, sizeof(MeshResource));
		Arr_PushBackRealloc_MeshResourceProvider(p_provider, &l_instanciatedResource);
		l_instanciatedResource->Key = l_key;
		ObjReader_loadMesh(p_meshSourceFilePath, &l_instanciatedResource->Mesh);

		*out_mesh = l_instanciatedResource;
	}
	(*out_mesh)->UsageCount += 1;
};

void MeshResourceProvider_ReleaseResource(MeshResource_Provider_PTR p_provider, MeshResource_KEY* p_key)
{
	MeshResource_HANDLE l_meshResource;
	size_t l_resourceIndex = MeshResourceProvider_GetResource(p_provider, p_key, &l_meshResource);
	if (l_meshResource)
	{
		(*l_meshResource).UsageCount -= 1;
		if ((*l_meshResource).UsageCount == 0)
		{ 
			Mesh_Free(&l_meshResource->Mesh);
			Arr_Erase_MeshResourceProvider(p_provider, l_resourceIndex);
			free(l_meshResource);
		}
	}
};