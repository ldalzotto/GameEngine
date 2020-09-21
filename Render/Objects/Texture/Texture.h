#pragma once

#include "Objects/Texture/Texture_def.h"
#include <stdbool.h>
#include "v2/_interface/RectC_def.h"

void Texture_Alloc_3C(Texture3c_PTR p_texture, uint16_t p_width, uint16_t p_height);
void Texture_Free_3C(Texture3c_PTR p_texture);

void Texture_Alloc_3f(Texture3f_PTR p_texture, uint16_t p_width, uint16_t p_height);
void Texture_Free_3f(Texture3f_PTR p_texture);

void Texture_Alloc_f(Texturef_PTR p_texture, uint16_t p_width, uint16_t p_height);
void Texture_Free_f(Texturef_PTR p_texture);

void Texture_AllocHeap_3c(uint16_t p_width, uint16_t p_height, Texture3c_HANDLE_PTR out_textureHandle);
void Texture_PushHeap_3c(Texture3c_PTR p_texture, Texture3c_HANDLE_PTR out_textureHandle);
void Texture_FreeHeap_3c(Texture3c_HANDLE_PTR p_textureHandle);

size_t Texture_GetSizeInBytes_3C(const Texture3c_PTR p_texture);
size_t Texture_GetElementOffset_3C(uint16_t W, uint16_t H, uint16_t textWidth);
void Texture_BuildClipRect_3C(Texture3c_PTR p_texture, Recti_PTR out_rect);
void Texture_CreateMemoryCursor_3C(Texture3c_PTR p_texture, Texture3c_MemoryCursor_PTR out_cursor);

void TextureMemCursor_MoveNextPixel_3C(Texture3c_MemoryCursor_PTR p_cursor);
bool TextureMemCursor_IsOutofBound_3C(Texture3c_MemoryCursor_PTR p_cursor);