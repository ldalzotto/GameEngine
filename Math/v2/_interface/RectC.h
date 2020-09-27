#pragma once

#include <stdbool.h>

#include "RectC_def.h"
#include "VectorC_def.h"

/* Returns True if the clipped segment is inside the rect */
bool Rect_ClipSegment_Int(const Vector2i_PTR in_clippedSegment_begin, const Vector2i_PTR in_clippedSegment_end,
	const Recti_PTR p_clippedRect, Vector2i_PTR out_clippedSegment_begin, Vector2i_PTR out_clippedSegment_end);

void Rect_ClipPoint_Int(const Recti_PTR p_clippedRect, Vector2i_PTR p_point);

RECT_INTERSECTION_TEST Intersection_Recti_Recti(const Recti_PTR p_left, const Recti_PTR p_right);