#pragma once

#include "Objects/Texture/Texture_def.h"
#include "v2/_interface/RectC_def.h"

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3f_PTR p_to, Recti_PTR p_clipRect,
	Color3f_PTR p_color);
