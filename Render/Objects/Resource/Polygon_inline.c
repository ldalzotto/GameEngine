#pragma once

#include "Polygon_def.h"

inline void _i_Polygon_Interpolate_Linear_V2F(const Polygon2f_PTR p_polygon, float l1, float l2, float l3, Vector2f_PTR out_interpolatedValue)
{
	out_interpolatedValue->x = (p_polygon->v1.x * l1) + (p_polygon->v2.x * l2) + (p_polygon->v3.x * l3);
	out_interpolatedValue->y = (p_polygon->v1.y * l1) + (p_polygon->v2.y * l2) + (p_polygon->v3.y * l3);
};

inline void _i_Polygon_Interpolate_Perspective_V2F(const Polygon2f_PTR p_polygon, float l1, float l2, float l3, PolygonPerspectiveInterpolation_PTR p_perspectiveInterpolation, Vector2f_PTR out_interpolatedValue)
{
	float l_I0_perspectiveCorrected = l1 * p_perspectiveInterpolation->InvertedZValueOnPolygon.v1;
	float l_I1_perspectiveCorrected = l2 * p_perspectiveInterpolation->InvertedZValueOnPolygon.v2;
	float l_I2_perspectiveCorrected = l3 * p_perspectiveInterpolation->InvertedZValueOnPolygon.v3;

	out_interpolatedValue->x = ((p_polygon->v1.x * l_I0_perspectiveCorrected) + (p_polygon->v2.x * l_I1_perspectiveCorrected) + (p_polygon->v3.x * l_I2_perspectiveCorrected)) * p_perspectiveInterpolation->ZValue;
	out_interpolatedValue->y = ((p_polygon->v1.y * l_I0_perspectiveCorrected) + (p_polygon->v2.y * l_I1_perspectiveCorrected) + (p_polygon->v3.y * l_I2_perspectiveCorrected)) * p_perspectiveInterpolation->ZValue;
};

inline void _i_Polygon_Interpolate_Perspective_V2i(const Polygon2i_PTR p_polygon, float l1, float l2, float l3, PolygonPerspectiveInterpolation_PTR p_perspectiveInterpolation, Vector2i_PTR out_interpolatedValue)
{
	float l_I0_perspectiveCorrected = l1 * p_perspectiveInterpolation->InvertedZValueOnPolygon.v1;
	float l_I1_perspectiveCorrected = l2 * p_perspectiveInterpolation->InvertedZValueOnPolygon.v2;
	float l_I2_perspectiveCorrected = l3 * p_perspectiveInterpolation->InvertedZValueOnPolygon.v3;

	out_interpolatedValue->x = ((p_polygon->v1.x * l_I0_perspectiveCorrected) + (p_polygon->v2.x * l_I1_perspectiveCorrected) + (p_polygon->v3.x * l_I2_perspectiveCorrected)) * p_perspectiveInterpolation->ZValue;
	out_interpolatedValue->y = ((p_polygon->v1.y * l_I0_perspectiveCorrected) + (p_polygon->v2.y * l_I1_perspectiveCorrected) + (p_polygon->v3.y * l_I2_perspectiveCorrected)) * p_perspectiveInterpolation->ZValue;
	// out_interpolatedValue->x = (int)((float)(((float)p_polygon->v1.x * l_I0_perspectiveCorrected) + ((float)p_polygon->v2.x * l_I1_perspectiveCorrected) + ((float)p_polygon->v3.x * l_I2_perspectiveCorrected)) * p_perspectiveInterpolation->ZValue);
	// out_interpolatedValue->y = (int)((float)(((float)p_polygon->v1.y * l_I0_perspectiveCorrected) + ((float)p_polygon->v2.y * l_I1_perspectiveCorrected) + ((float)p_polygon->v3.y * l_I2_perspectiveCorrected)) * p_perspectiveInterpolation->ZValue);
};