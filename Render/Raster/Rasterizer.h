#pragma once

#include "Rasterizer_def.h"
#include <stdbool.h>
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"

bool Rasterize_LineClipped(
	const VECTOR2F_PTR p_begin, const VECTOR2F_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR out_rasterizedPixels,
	const RECTI_PTR p_clip_rect,
	VECTOR2I_PTR out_clipped_begin,
	VECTOR2I_PTR out_clipped_end);

inline void Arr_Alloc_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array, size_t p_initialSize) { Arr_Alloc((ARRAY_PTR)p_array, sizeof(RASTERIZATIONSTEP), p_initialSize); }
inline void Arr_Free_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array) { Arr_Free((ARRAY_PTR)p_array); }

inline void Arr_Clear_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array) { Arr_Clear((ARRAY_PTR)p_array); };
inline void Arr_Resize_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array, size_t p_newCapacity) { HANDLE_ERR(Arr_Resize((ARRAY_PTR)p_array, sizeof(RASTERIZATIONSTEP), p_newCapacity)); };