#include "Texture.h"
#include "v2/_interface/VectorStructuresC.h"

void Texture_Alloc_3C(TEXTURE3C_PTR p_texture, size_t p_width, size_t p_height)
{
	p_texture->Width = p_width;
	p_texture->Height = p_height;
	Arr_Alloc_Vector3C(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
	p_texture->Pixels.Size = p_texture->Pixels.Capacity;
};

void Texture_Free_3C(TEXTURE3C_PTR p_texture)
{
	Arr_Free_Vector3C(&p_texture->Pixels);
};

size_t Texture_GetSizeInBytes_3C(const TEXTURE3C_PTR p_texture)
{
	return ((size_t)p_texture->Width * p_texture->Height) * (3 * sizeof(char));
};

size_t Texture_GetElementOffset_3C(int W, int H, int textWidth)
{
	return (W * sizeof(VECTOR3C)) + (H * textWidth * sizeof(VECTOR3C));
}

void Texture_CuildClipRect_3C(TEXTURE3C_PTR p_texture, RECTI_PTR out_rect)
{
	*out_rect = (RECTI) { {0,0}, {(int)p_texture->Width - 1, (int)p_texture->Height - 1} };
};

void Texture_CreateMemoryCursor_3C(TEXTURE3C_PTR p_texture, TEXTURE3C_MEMORYCURSOR_PTR out_cursor)
{
	*out_cursor = (TEXTURE3C_MEMORYCURSOR) { .Texture = p_texture, .Current = p_texture->Pixels.Memory, .CurrentIndex = 0 };
};

void TextureMemCursor_MoveNextPixel_3C(TEXTURE3C_MEMORYCURSOR_PTR p_cursor)
{
	p_cursor->CurrentIndex += 1;
	p_cursor->Current = (VECTOR3C_PTR)((char*)p_cursor->Current + sizeof(VECTOR3C));
};

bool TextureMemCursor_IsOutofBound_3C(TEXTURE3C_MEMORYCURSOR_PTR p_cursor)
{
	return p_cursor->CurrentIndex >= p_cursor->Texture->Pixels.Size;
};