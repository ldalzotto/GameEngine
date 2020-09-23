#pragma once

#include "Texture_def.h"
#include "v2/_inline/ColorC_inline.c"

inline void _i_TextureSample_Point_3f(const Texture3c_PTR p_sampledTexture, Vector2f_PTR p_uvSample, Color3f_PTR out_sampledColor)
{
	_i_Color_Convert_3C_3F(&p_sampledTexture->Pixels.Memory
		[(int)((p_sampledTexture->Width - 1) * p_uvSample->x) + (p_sampledTexture->Width * (int)((p_sampledTexture->Height - 1) * (1.0f - p_uvSample->y)))], out_sampledColor);
};
