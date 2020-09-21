#include "Texture.h"
// #include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/ColorC.h"
#include "Heap/RenderHeap.h"

void Texture_Alloc_3C(Texture3c_PTR p_texture, uint16_t p_width, uint16_t p_height)
{
	p_texture->Width = p_width;
	p_texture->Height = p_height;
	Arr_Alloc_Color3c(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
	p_texture->Pixels.Size = p_texture->Pixels.Capacity;
};

void Texture_Free_3C(Texture3c_PTR p_texture)
{
	Arr_Free(&p_texture->Pixels.array);
};

void Texture_Alloc_3f(Texture3f_PTR p_texture, uint16_t p_width, uint16_t p_height)
{
	p_texture->Width = p_width;
	p_texture->Height = p_height;
	Arr_Alloc_Color3f(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
	p_texture->Pixels.Size = p_texture->Pixels.Capacity;
};

void Texture_Free_3f(Texture3f_PTR p_texture)
{
	Arr_Free(&p_texture->Pixels.array);
};

void Texture_Alloc_f(Texturef_PTR p_texture, uint16_t p_width, uint16_t p_height)
{
	p_texture->Width = p_width;
	p_texture->Height = p_height;
	Arr_Alloc_Float(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
	p_texture->Pixels.Size = p_texture->Pixels.Capacity;
};

void Texture_Free_f(Texturef_PTR p_texture)
{
	Arr_Free(&p_texture->Pixels.array);
};

void Texture_AllocHeap_3c(uint16_t p_width, uint16_t p_height, Texture3c_HANDLE_PTR out_textureHandle)
{
	PoolAllocator_AllocElement_Texture3c(&RRenderHeap.Texture3cAllocator, out_textureHandle);
	Texture_Alloc_3C(&RRenderHeap.Texture3cAllocator.array.Memory[out_textureHandle->Handle], p_width, p_height);
};

void Texture_PushHeap_3c(Texture3c_PTR p_texture, Texture3c_HANDLE_PTR out_textureHandle)
{
	PoolAllocator_AllocElement_Texture3c(&RRenderHeap.Texture3cAllocator, out_textureHandle);
	RRenderHeap.Texture3cAllocator.array.Memory[out_textureHandle->Handle] = *p_texture;
};

void Texture_FreeHeap_3c(Texture3c_HANDLE_PTR p_textureHandle)
{
	Texture_Free_3C(&RRenderHeap.Texture3cAllocator.array.Memory[p_textureHandle->Handle]);
	PoolAllocator_FreeElement_Texture3c(&RRenderHeap.Texture3cAllocator, *p_textureHandle);
};

size_t Texture_GetSizeInBytes_3C(const Texture3c_PTR p_texture)
{
	return ((size_t)p_texture->Width * p_texture->Height) * (3 * sizeof(char));
};

size_t Texture_GetElementOffset_3C(uint16_t W, uint16_t H, uint16_t textWidth)
{
	return (W * sizeof(Vector3c)) + ((size_t)H * textWidth * sizeof(Vector3c));
}

void Texture_BuildClipRect_3C(Texture3c_PTR p_texture, Recti_PTR out_rect)
{
	*out_rect = (Recti) { {0,0}, {(int)p_texture->Width - 1, (int)p_texture->Height - 1} };
};

void Texture_CreateMemoryCursor_3C(Texture3c_PTR p_texture, Texture3c_MemoryCursor_PTR out_cursor)
{
	*out_cursor = (Texture3c_MemoryCursor) { .Texture = p_texture, .Current = p_texture->Pixels.Memory, .CurrentIndex = 0 };
};

void TextureMemCursor_MoveNextPixel_3C(Texture3c_MemoryCursor_PTR p_cursor)
{
	p_cursor->CurrentIndex += 1;
	p_cursor->Current = (Color3c_PTR)((char*)p_cursor->Current + sizeof(Color3c));
};

bool TextureMemCursor_IsOutofBound_3C(Texture3c_MemoryCursor_PTR p_cursor)
{
	return p_cursor->CurrentIndex >= p_cursor->Texture->Pixels.Size;
};