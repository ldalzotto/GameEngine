#pragma once

#include "v2/_inline/Math_inline.c"
#include "v2/_interface/ColorC.h"

inline void _i_Color_Convert_3F_3C(Color3f_PTR p_color3f, Color3c_PTR p_color3c)
{
	p_color3c->r = (char)(_i_Math_clamp1f(p_color3f->b) * 255.0f);
	p_color3c->g = (char)(_i_Math_clamp1f(p_color3f->g) * 255.0f);
	p_color3c->b = (char)(_i_Math_clamp1f(p_color3f->r) * 255.0f);
};

inline void _i_Color_Convert_3C_3F(Color3c_PTR p_color3c, Color3f_PTR p_color3f)
{
	p_color3f->r = ((float)p_color3c->r * 0.0039156862f);
	p_color3f->g = ((float)p_color3c->g * 0.0039156862f);
	p_color3f->b = ((float)p_color3c->b * 0.0039156862f);
};