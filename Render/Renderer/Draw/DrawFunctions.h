#pragma once

#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/VectorStructuresC_def.h"
#include "Objects/Resource/Polygon_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Raster/Rasterizer_def.h"


void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR in_out_rasterizedPixelsBuffer,
	Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Vector3c_PTR p_color);

void Draw_PolygonClipped(Polygon2i_PTR p_polygon, Array_Vector2i_PTR p_rasterizedPixelBuffer, Texture3c_PTR p_to, Recti_PTR p_clipRect, Vector3c_PTR p_color);