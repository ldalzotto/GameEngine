#include "TextureResourceProvider.h"

#include "Texture.h"
#include "Heap/RenderHeap_def.h"
#include "DataStructures/ARRAY.h"
#include "Functional/Hash.h"
#include "File/TextureReader.h"

ARRAY_ALLOC_FUNCTION(TextureResourcePTR, Array_TextureResourceHandle_PTR, TextureResource_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(TextureResourcePTR, Array_TextureResourceHandle_PTR, TextureResource_PTR)
ARRAY_ERASE_FUNCTION(TextureResourcePTR, Array_TextureResourceHandle_PTR, TextureResource_PTR)

void TextureResourceProvider_Alloc(Array_TextureResourceHandle_PTR p_provider)
{
	Arr_Alloc_TextureResourcePTR(p_provider, 0);
};

void TextureResourceProvider_Free(Array_TextureResourceHandle_PTR p_provider)
{
	for (size_t i = 0; i < p_provider->Size; i++)
	{
		Texture_FreeHeap_3c(&p_provider->Memory[i]->Texture);
	}
	Arr_Free(&p_provider->array);
};



size_t TextureResourceProvider_GetResource(Array_TextureResourceHandle_PTR p_provider, TextureResource_KEY* p_key, TextureResource_PTR* out_mesh)
{
	for (size_t i = 0; i < p_provider->Size; i++)
	{
		TextureResource_PTR l_textureResource = p_provider->Memory[i];
		if (l_textureResource->Key == *p_key)
		{
			*out_mesh = l_textureResource;
			return i;
		}
	}
	*out_mesh = NULL;
	return -1;
};

void TextureResourceProvider_UseResource(Array_TextureResourceHandle_PTR p_provider, Assetpath_PTR p_texutreSourceFile, TextureResource_PTR* out_textureResource)
{
	TextureResource_KEY l_key = HashCombine_string(0, p_texutreSourceFile->Path);
	TextureResourceProvider_GetResource(p_provider, &l_key, out_textureResource);
	if (!*out_textureResource)
	{
		TextureResource_PTR l_instanciatedResource = (TextureResource_PTR)calloc(1, sizeof(TextureResource));
		Arr_PushBackRealloc_TextureResourcePTR(p_provider, &l_instanciatedResource);
		l_instanciatedResource->Key = l_key;

		Texture3c l_loadedTexture;
		TextureReader_load(p_texutreSourceFile->Path, &l_loadedTexture);
		Texture_PushHeap_3c(&l_loadedTexture, &l_instanciatedResource->Texture);
		*out_textureResource = l_instanciatedResource;
	}
	(*out_textureResource)->UsageCount += 1;
};

void TextureResourceProvider_ReleaseResource(Array_TextureResourceHandle_PTR p_provider, TextureResource_KEY* p_key)
{
	TextureResource_PTR l_textureResource;
	size_t l_resourceIndex = TextureResourceProvider_GetResource(p_provider, p_key, &l_textureResource);
	if (l_textureResource)
	{
		l_textureResource->UsageCount -= 1;
		if (l_textureResource->UsageCount == 0)
		{
			Texture_FreeHeap_3c(&l_textureResource->Texture);
			Arr_Erase_TextureResourcePTR(p_provider, l_resourceIndex);
			free(l_textureResource);
		}
	}
};
