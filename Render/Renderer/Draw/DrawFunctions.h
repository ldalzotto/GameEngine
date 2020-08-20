#pragma once


#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Raster/Rasterizer_def.h"


void Draw_LineClipped(
	VECTOR2F_PTR p_begin, VECTOR2F_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR in_out_rasterizedPixelsBuffer,
	TEXTURE3C_PTR p_to, RECTI_PTR p_clipRect,
	VECTOR3C_PTR p_color);
