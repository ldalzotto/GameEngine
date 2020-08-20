#pragma once

#include "Texture_def.h"
#include <stdbool.h>
#include "v2/_interface/RectC_def.h"

void Texture_Alloc_3C(TEXTURE3C_PTR p_texture, size_t p_width, size_t p_height);
void Texture_Free_3C(TEXTURE3C_PTR p_texture);
size_t Texture_GetSizeInBytes_3C(const TEXTURE3C_PTR p_texture);
size_t Texture_GetElementOffset_3C(int W, int H, int textWidth);
void Texture_CuildClipRect_3C(TEXTURE3C_PTR p_texture, RECTI_PTR out_rect);
void Texture_CreateMemoryCursor_3C(TEXTURE3C_PTR p_texture, TEXTURE3C_MEMORYCURSOR_PTR out_cursor);

void TextureMemCursor_MoveNextPixel_3C(TEXTURE3C_MEMORYCURSOR_PTR p_cursor);
bool TextureMemCursor_IsOutofBound_3C(TEXTURE3C_MEMORYCURSOR_PTR p_cursor);