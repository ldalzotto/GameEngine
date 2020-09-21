#pragma once

#include "Texture_def.h"
#include "DataStructures/ARRAY_def.h"
#include "Asset/AssetPaths.h"

typedef size_t TextureResource_KEY;

typedef struct TextureResource_TYP
{
	TextureResource_KEY Key;
	Texture3c_HANDLE Texture;
	size_t UsageCount;
}TextureResource, * TextureResource_PTR;

typedef struct Array_TextureResourceHandle_TYP
{
	ARRAY_TYPE_DEFINITION(TextureResource_PTR)
}Array_TextureResourceHandle, * Array_TextureResourceHandle_PTR;

void TextureResourceProvider_Alloc(Array_TextureResourceHandle_PTR p_provider);
void TextureResourceProvider_Free(Array_TextureResourceHandle_PTR p_provider);
void TextureResourceProvider_UseResource(Array_TextureResourceHandle_PTR p_provider, Assetpath_PTR p_texutreSourceFile, TextureResource_PTR* out_textureResource);
void TextureResourceProvider_ReleaseResource(Array_TextureResourceHandle_PTR p_provider, TextureResource_KEY* p_key);