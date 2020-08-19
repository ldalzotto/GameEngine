#pragma once

#include <stdbool.h>

#include "RectC_def.h"
#include "VectorC_def.h"

bool Rect_ClipSegment_Int(const VECTOR2F_PTR in_clippedSegment_begin, const VECTOR2F_PTR in_clippedSegment_end,
	const RECTI_PTR p_clippedRect, VECTOR2I_PTR out_clippedSegment_begin, VECTOR2I_PTR out_clippedSegment_end);