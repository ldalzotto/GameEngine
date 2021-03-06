#include "Rasterizer.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "Constants.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/RectC.h"
#include "Error/ErrorHandler.h"


// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
char LineRasterize_Initialize(const Vector2i_PTR p_begin, const Vector2i_PTR p_end, const Recti_PTR p_clip_rect, LineRasterizerIterator_PTR out_lineRasterizerIterator)
{
	Vector2i l_clipped_begin, l_clipped_end;
	if (Rect_ClipSegment_Int(p_begin, p_end, p_clip_rect, &l_clipped_begin, &l_clipped_end))
	{
		out_lineRasterizerIterator->CurrentPoint = l_clipped_begin;
		out_lineRasterizerIterator->EndPoint = l_clipped_end;
		out_lineRasterizerIterator->dx = abs(l_clipped_end.x - l_clipped_begin.x); out_lineRasterizerIterator->sx = l_clipped_begin.x < l_clipped_end.x ? 1 : -1;
		out_lineRasterizerIterator->dy = -abs(l_clipped_end.y - l_clipped_begin.y); out_lineRasterizerIterator->sy = l_clipped_begin.y < l_clipped_end.y ? 1 : -1;
		out_lineRasterizerIterator->err = out_lineRasterizerIterator->dx + out_lineRasterizerIterator->dy;
		out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_BEGIN;
		return 1;
	};
	return 0;
};

char LineRasterize_MoveNext(LineRasterizerIterator_PTR out_lineRasterizerIterator)
{
	switch (out_lineRasterizerIterator->CurrentStep)
	{
	case LINERASTERIZER_ITERATOR_STEP_BEGIN:
	{
		out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_LOOPING;
		return 1;
	}
	break;
	case LINERASTERIZER_ITERATOR_STEP_LOOPING:
	{
		int e2 = 2 * out_lineRasterizerIterator->err;
		if (e2 >= out_lineRasterizerIterator->dy)
		{
			if (out_lineRasterizerIterator->CurrentPoint.x == out_lineRasterizerIterator->EndPoint.x)
			{
				out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_END;
				return 1;
			}
			out_lineRasterizerIterator->err += out_lineRasterizerIterator->dy;
			out_lineRasterizerIterator->CurrentPoint.x += out_lineRasterizerIterator->sx;
		}
		if (e2 <= out_lineRasterizerIterator->dx)
		{
			if (out_lineRasterizerIterator->CurrentPoint.y == out_lineRasterizerIterator->EndPoint.y)
			{
				out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_END;
				return 1;
			}
			out_lineRasterizerIterator->err += out_lineRasterizerIterator->dx;
			out_lineRasterizerIterator->CurrentPoint.y += out_lineRasterizerIterator->sy;
		}
		return 1;
	}
	break;
	case LINERASTERIZER_ITERATOR_STEP_END:
	{
		return 0;
	}
	break;
	}

	return 0;
};

inline int _i_PolygonRasterizePackedData_EdgeFunction_0(const PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon, const Vector2i_PTR p_point)
{
	return ((p_point->x - p_polygon->v1.x) * p_packedData->dy0) - ((p_point->y - p_polygon->v1.y) * p_packedData->dx0);
};

inline int _i_PolygonRasterizePackedData_EdgeFunction_1(const PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon, const Vector2i_PTR p_point)
{
	return ((p_point->x - p_polygon->v2.x) * p_packedData->dy1) - ((p_point->y - p_polygon->v2.y) * p_packedData->dx1);
};

inline int _i_PolygonRasterizePackedData_EdgeFunction_2(const PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon, const Vector2i_PTR p_point)
{
	return ((p_point->x - p_polygon->v3.x) * p_packedData->dy2) - ((p_point->y - p_polygon->v3.y) * p_packedData->dx2);
};

inline void _i_PolygonRasterizePackedData_PrecalculateRasterization(PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon)
{

	p_packedData->dx0 = p_polygon->v1.x - p_polygon->v3.x;
	p_packedData->dy0 = p_polygon->v1.y - p_polygon->v3.y;

	p_packedData->dx1 = p_polygon->v2.x - p_polygon->v1.x;
	p_packedData->dy1 = p_polygon->v2.y - p_polygon->v1.y;

	p_packedData->dx2 = p_polygon->v3.x - p_polygon->v2.x;
	p_packedData->dy2 = p_polygon->v3.y - p_polygon->v2.y;

	Vector3f l_begin = { (float)p_polygon->v2.x - p_polygon->v1.x, (float)p_polygon->v2.y - p_polygon->v1.y, 0.0f };
	Vector3f l_end = { (float)p_polygon->v3.x - p_polygon->v1.x, (float)p_polygon->v3.y - p_polygon->v1.y, 0.0f };
	Vector3f l_crossResult;
	Vec_Cross_3f(&l_begin, &l_end, &l_crossResult);

	// Anti-clockwise
	if (l_crossResult.z > 0.0f)
	{
		p_packedData->dx0 *= -1;
		p_packedData->dy0 *= -1;
		p_packedData->dx1 *= -1;
		p_packedData->dy1 *= -1;
		p_packedData->dx2 *= -1;
		p_packedData->dy2 *= -1;
	}
};

inline void _i_PolygonRasterizePackedData_PrecalculateAreas(PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon)
{
	p_packedData->area0 = _i_PolygonRasterizePackedData_EdgeFunction_0(p_packedData, p_polygon, &p_polygon->v2);
	p_packedData->area1 = _i_PolygonRasterizePackedData_EdgeFunction_1(p_packedData, p_polygon, &p_polygon->v3);
	p_packedData->area2 = _i_PolygonRasterizePackedData_EdgeFunction_2(p_packedData, p_polygon, &p_polygon->v1);
};

inline void _i_PolygonIterator_CommonStructure_Initialize(PolygonRasterizerIterator_CommonStructure_PTR p_commonStructure, const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect)
{
	Polygon_BoundingRect_2i(p_polygon, &p_commonStructure->PolygonBoundClip);
	Rect_ClipPoint_Int(p_clip_rect, &p_commonStructure->PolygonBoundClip.Min);
	Rect_ClipPoint_Int(p_clip_rect, &p_commonStructure->PolygonBoundClip.Max);
	_i_PolygonRasterizePackedData_PrecalculateRasterization(&p_commonStructure->PackedRasterizerData, p_polygon);
	_i_PolygonRasterizePackedData_PrecalculateAreas(&p_commonStructure->PackedRasterizerData, p_polygon);
}

void PolygonRasterize_Initialize(const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect, PolygonRasterizerIterator_PTR out_polygonRasterizerIterator)
{
	_i_PolygonIterator_CommonStructure_Initialize(&out_polygonRasterizerIterator->CommonStructure, p_polygon, p_clip_rect);

	out_polygonRasterizerIterator->CurrentPoint = out_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Min;
	out_polygonRasterizerIterator->CurrentPoint.y -= 1;

	out_polygonRasterizerIterator->e0 = _i_PolygonRasterizePackedData_EdgeFunction_0(&out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CurrentPoint);
	out_polygonRasterizerIterator->e1 = _i_PolygonRasterizePackedData_EdgeFunction_1(&out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CurrentPoint);
	out_polygonRasterizerIterator->e2 = _i_PolygonRasterizePackedData_EdgeFunction_2(&out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CurrentPoint);


	//This check is done to avoid drawing polygon which have two points that are the same.
	// Because this cause artifacts
	if ((out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.area0 +
		out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.area1 +
		out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.area2) <= FLOAT_TOLERANCE)
	{
		out_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_EXIT;
	}
	else
	{
		out_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE;
	}
}

// For interpolation, we can use e1, e2, e0 as they represent the already calculated edge functions for the current pixel
inline void _i_PolygonRasterize_Interpolate(PolygonRasterizerIterator_CommonStructure_PTR p_polygonRasterizerIterator, int p_edgeFunctions[3])
{
	p_polygonRasterizerIterator->InterpolationFactors.I0 = ((float)p_edgeFunctions[2] / (float)p_polygonRasterizerIterator->PackedRasterizerData.area0);
	p_polygonRasterizerIterator->InterpolationFactors.I1 = ((float)p_edgeFunctions[0] / (float)p_polygonRasterizerIterator->PackedRasterizerData.area1);
	p_polygonRasterizerIterator->InterpolationFactors.I2 = ((float)p_edgeFunctions[1] / (float)p_polygonRasterizerIterator->PackedRasterizerData.area2);
};

//A Parallel Algorithm for Polygon Rasterization.pdf
// This polygon rasterize iterator iterate over the bounding box of the Polygon and rasterize the pixel is inside the polygon.
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterize_MoveNext_Interpolated(PolygonRasterizerIterator_PTR p_polygonRasterizerIterator)
{
	/*PolygonRasterize_MoveNext_Algorithm_Interpolation(PolygonRasterize_MoveNext_Interpolated)*/ 
	switch (p_polygonRasterizerIterator->CurrentStep)
	{
	case POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE:
	{
		p_polygonRasterizerIterator->CurrentPoint.y += 1;
		if (p_polygonRasterizerIterator->CurrentPoint.y > p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Max.y)
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN;
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END;
		}
		p_polygonRasterizerIterator->e0 -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dx0;
		p_polygonRasterizerIterator->e1 -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dx1;
		p_polygonRasterizerIterator->e2 -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dx2;
		p_polygonRasterizerIterator->LineBeginPoint = p_polygonRasterizerIterator->CurrentPoint;
		memcpy(&p_polygonRasterizerIterator->line_e0, &p_polygonRasterizerIterator->e0, sizeof(int) * 3);
		p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN;
		if (p_polygonRasterizerIterator->e0 >= 0 && p_polygonRasterizerIterator->e1 >= 0 && p_polygonRasterizerIterator->e2 >= 0)
		{
			p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->CurrentPoint;
			_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->e0);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
		}
		else
		{
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
	}
	break;
	case 2:
	{
		p_polygonRasterizerIterator->CurrentPoint.x += 1;
		if (p_polygonRasterizerIterator->CurrentPoint.x > p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Max.x)
		{
			p_polygonRasterizerIterator->CurrentPoint = p_polygonRasterizerIterator->LineBeginPoint;
			memcpy(&p_polygonRasterizerIterator->e0, &p_polygonRasterizerIterator->line_e0, sizeof(int) * 3);
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE;
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
		p_polygonRasterizerIterator->e0 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy0;
		p_polygonRasterizerIterator->e1 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy1;
		p_polygonRasterizerIterator->e2 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy2;
		if (p_polygonRasterizerIterator->e0 >= 0 && p_polygonRasterizerIterator->e1 >= 0 && p_polygonRasterizerIterator->e2 >= 0)
		{
			p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->CurrentPoint;
			_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->e0);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
		}
		else
		{
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
	}
	break;
	}
	return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END;
};

void PolygonRasterizeSmart_Initialize(const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect, PolygonRasterizeSmartIterator_PTR out_polygonRasterizerIterator)
{

	_i_PolygonIterator_CommonStructure_Initialize(&out_polygonRasterizerIterator->CommonStructure, p_polygon, p_clip_rect);

	//This check is done to avoid drawing polygon which have two points that are the same.
	// Because this cause artifacts
	if ((out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.area0 +
		out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.area1 +
		out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.area2) <= FLOAT_TOLERANCE)
	{
		out_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_NOTHING;
	}
	else
	{
		//We get the top vertex
		short int l_topVertexIndex = 0;
		for (short int i = 1; i < 3; i++)
		{
			if (p_polygon->Points[i].y >= p_polygon->Points[l_topVertexIndex].y) {
				l_topVertexIndex = i;
			}
		}

		out_polygonRasterizerIterator->CenterScanCursor = p_polygon->Points[l_topVertexIndex];
		Rect_ClipPoint_Int(p_clip_rect, &out_polygonRasterizerIterator->CenterScanCursor);
		out_polygonRasterizerIterator->CenterScanCursor.y += 1;

		out_polygonRasterizerIterator->Center_e0 = _i_PolygonRasterizePackedData_EdgeFunction_0(&out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CenterScanCursor);
		out_polygonRasterizerIterator->Center_e1 = _i_PolygonRasterizePackedData_EdgeFunction_1(&out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CenterScanCursor);
		out_polygonRasterizerIterator->Center_e2 = _i_PolygonRasterizePackedData_EdgeFunction_2(&out_polygonRasterizerIterator->CommonStructure.PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CenterScanCursor);

		out_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND;
	}
};

//A Parallel Algorithm for Polygon Rasterization.pdf
// This smart polygon rasterizer iterator go from top to bottom while staying inside the polygon. For each line, we find if any pixels to left and right are inside the Polygon, and rasterize them.
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterizeSmart_MoveNext_Interpolated(PolygonRasterizeSmartIterator_PTR p_polygonRasterizerIterator)
{
	switch (p_polygonRasterizerIterator->CurrentStep)
	{
	case POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND:
	{ 
		/* We move the center cursor down */
		p_polygonRasterizerIterator->CenterScanCursor.y -= 1;
		if (p_polygonRasterizerIterator->CenterScanCursor.y < p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Min.y)
		{
			break;
		} 
		/* We go down one pixel */
		p_polygonRasterizerIterator->Center_e0 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dx0;
		p_polygonRasterizerIterator->Center_e1 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dx1;
		p_polygonRasterizerIterator->Center_e2 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dx2; 
		/* Is the value iside the polygon ? */
		if (p_polygonRasterizerIterator->Center_e0 >= 0 && p_polygonRasterizerIterator->Center_e1 >= 0 && p_polygonRasterizerIterator->Center_e2 >= 0)
		{
			p_polygonRasterizerIterator->LineScanCursor = p_polygonRasterizerIterator->CenterScanCursor;
			memcpy(&p_polygonRasterizerIterator->Line_e0, &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_LEFT;
			p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->CenterScanCursor;
			_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->Center_e0);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
		} 
		/* Else, maybe that the center is at the same line */
		else
		{
			int l_lineIndex = p_polygonRasterizerIterator->CenterScanCursor.x - 1;
			int l_left_e[3];
			memcpy(&l_left_e[0], &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
			while (l_lineIndex >= p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Min.x)
			{
				l_left_e[0] -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy0;
				l_left_e[1] -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy1;
				l_left_e[2] -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy2;
				if (l_left_e[0] >= 0 && l_left_e[1] >= 0 && l_left_e[2] >= 0)
				{
					memcpy(&p_polygonRasterizerIterator->Center_e0, &l_left_e[0], sizeof(int) * 3);
					p_polygonRasterizerIterator->CenterScanCursor.x = l_lineIndex;
					p_polygonRasterizerIterator->LineScanCursor = p_polygonRasterizerIterator->CenterScanCursor;
					memcpy(&p_polygonRasterizerIterator->Line_e0, &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
					p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_LEFT;
					p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->CenterScanCursor;
					_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->Center_e0);
					return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
				}
				l_lineIndex -= 1;
			}
			l_lineIndex = p_polygonRasterizerIterator->CenterScanCursor.x + 1;
			memcpy(&l_left_e[0], &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
			while (l_lineIndex <= p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Max.x)
			{
				l_left_e[0] += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy0;
				l_left_e[1] += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy1;
				l_left_e[2] += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy2;
				if (l_left_e[0] >= 0 && l_left_e[1] >= 0 && l_left_e[2] >= 0)
				{
					memcpy(&p_polygonRasterizerIterator->Center_e0, &l_left_e[0], sizeof(int) * 3);
					p_polygonRasterizerIterator->CenterScanCursor.x = l_lineIndex;
					p_polygonRasterizerIterator->LineScanCursor = p_polygonRasterizerIterator->CenterScanCursor;
					memcpy(&p_polygonRasterizerIterator->Line_e0, &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
					p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_LEFT;
					p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->CenterScanCursor;
					_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->Center_e0);
					return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
				}
				l_lineIndex += 1;
			}
		} /* Else, we try to find the center of the next line */
		p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND;
		return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	}	  /* The first step of the line scan line is going to left from center. Once the scan has reached the end, we go to POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_RIGHT */
	case POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_LEFT:
	{
		p_polygonRasterizerIterator->LineScanCursor.x -= 1;
		if (p_polygonRasterizerIterator->LineScanCursor.x < p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Min.x)
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_RIGHT;
			p_polygonRasterizerIterator->LineScanCursor = p_polygonRasterizerIterator->CenterScanCursor;
			memcpy(&p_polygonRasterizerIterator->Line_e0, &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
		p_polygonRasterizerIterator->Line_e0 -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy0;
		p_polygonRasterizerIterator->Line_e1 -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy1;
		p_polygonRasterizerIterator->Line_e2 -= p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy2;
		if (p_polygonRasterizerIterator->Line_e0 >= 0 && p_polygonRasterizerIterator->Line_e1 >= 0 && p_polygonRasterizerIterator->Line_e2 >= 0)
		{
			p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->LineScanCursor;
			_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->Line_e0);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
		}
		else
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_RIGHT;
			p_polygonRasterizerIterator->LineScanCursor = p_polygonRasterizerIterator->CenterScanCursor;
			memcpy(&p_polygonRasterizerIterator->Line_e0, &p_polygonRasterizerIterator->Center_e0, sizeof(int) * 3);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
	} /* The last step of a scan line is going right from center. When we have reached the end, we try to find a new center POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND. */
	case POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_RIGHT:
	{
		p_polygonRasterizerIterator->LineScanCursor.x += 1;
		if (p_polygonRasterizerIterator->LineScanCursor.x > p_polygonRasterizerIterator->CommonStructure.PolygonBoundClip.Max.x)
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND;
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
		p_polygonRasterizerIterator->Line_e0 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy0;
		p_polygonRasterizerIterator->Line_e1 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy1;
		p_polygonRasterizerIterator->Line_e2 += p_polygonRasterizerIterator->CommonStructure.PackedRasterizerData.dy2;
		if (p_polygonRasterizerIterator->Line_e0 >= 0 && p_polygonRasterizerIterator->Line_e1 >= 0 && p_polygonRasterizerIterator->Line_e2 >= 0)
		{
			p_polygonRasterizerIterator->CommonStructure.RasterizedPixel = p_polygonRasterizerIterator->LineScanCursor;
			_i_PolygonRasterize_Interpolate(&p_polygonRasterizerIterator->CommonStructure, &p_polygonRasterizerIterator->Line_e0);
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
		}
		else
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND;
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		}
	}
	break;
	}
	return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END;
};