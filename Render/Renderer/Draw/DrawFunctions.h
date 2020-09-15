#pragma once

#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/VectorStructuresC_def.h"
#include "Objects/Resource/Polygon_def.h"
#include "Objects/Texture/Texture_def.h"


void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Vector3c_PTR p_color);

void Draw_PolygonClipped(Polygon2i_PTR p_polygon, Texture3c_PTR p_to, Recti_PTR p_clipRect, Vector3c_PTR p_color);