#pragma once

#include <stdbool.h>

#include "RectC_def.h"
#include "VectorC_def.h"

/* Returns True if the clipped segment is inside the rect */
bool Rect_ClipSegment_Int(const Vector2f_PTR in_clippedSegment_begin, const Vector2f_PTR in_clippedSegment_end,
	const Recti_PTR p_clippedRect, Vector2i_PTR out_clippedSegment_begin, Vector2i_PTR out_clippedSegment_end);