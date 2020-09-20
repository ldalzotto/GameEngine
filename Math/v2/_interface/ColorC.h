#pragma once

#include "ColorC_def.h"
#include "DataStructures/ARRAY.h"

void Color_Convert_3F_3C(Color3f_PTR p_color3f, Color3c_PTR p_color3c);

inline void Color_Convert_3C_3F_inline(Color3c_PTR p_color3c, Color3f_PTR p_color3f)
{
	p_color3f->r = ((float)p_color3c->r * 0.0039156862f);
	p_color3f->g = ((float)p_color3c->g * 0.0039156862f);
	p_color3f->b = ((float)p_color3c->b * 0.0039156862f);
};

ARRAY_ALLOC_FUNCTION(Color3c, Array_Color3c_PTR, Color3c)