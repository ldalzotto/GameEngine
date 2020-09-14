#pragma once

#include "Rasterizer_def.h"
#include <stdbool.h>
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"
#include "Objects/Resource/Polygon_def.h"
#include "v2/_interface/VectorStructuresC_def.h"

bool Rasterize_LineClipped(
	const Vector2i_PTR p_begin, const Vector2i_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR out_rasterizedPixels,
	const Recti_PTR p_clip_rect,
	Vector2i_PTR out_clipped_begin,
	Vector2i_PTR out_clipped_end);

void Rasterize_PolygonClipped(const Polygon2i_PTR p_polygon, Array_Vector2i_PTR out_rasterizedPixels, const Recti_PTR p_clip_rect);

inline void Arr_Alloc_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array, size_t p_initialSize) { Arr_Alloc(&p_array->array, sizeof(RASTERIZATIONSTEP), p_initialSize); }
inline void Arr_Free_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array) { Arr_Free(&p_array->array); }

inline void Arr_Clear_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array) { Arr_Clear(&p_array->array); };
inline void Arr_Resize_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array, size_t p_newCapacity) { HANDLE_ERR(Arr_Resize(&p_array->array, sizeof(RASTERIZATIONSTEP), p_newCapacity)); };