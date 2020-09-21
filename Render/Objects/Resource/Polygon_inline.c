#pragma once

#include "Polygon_def.h"

inline void _i_Polygon_Interpolate_V2F(const Polygon2f_PTR p_polygon, float l1, float l2, float l3, Vector2f_PTR out_interpolatedValue)
{
	out_interpolatedValue->x = (p_polygon->v1.x * l1) + (p_polygon->v2.x * l2) + (p_polygon->v3.x * l3);
	out_interpolatedValue->y = (p_polygon->v1.y * l1) + (p_polygon->v2.y * l2) + (p_polygon->v3.y * l3);
};