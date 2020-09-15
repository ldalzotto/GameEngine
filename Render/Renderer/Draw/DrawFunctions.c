#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"

#include "Objects/Texture/Texture.h"

inline void EvaluatePixelColor(Texture3c_PTR p_to, Vector2i_PTR p_pixelCoord, Material_PTR p_material)
{
	p_to->Pixels.Memory[p_pixelCoord->x + (p_pixelCoord->y * p_to->Width)] = p_material->BaseColor;
};

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Vector3c_PTR p_color)
{
	LineRasterizerIterator l_lineRasterizerIterator;
	if (LineRasterize_Initialize(p_begin, p_end, p_clipRect, &l_lineRasterizerIterator))
	{
		while (LineRasterize_MoveNext(&l_lineRasterizerIterator))
		{
			p_to->Pixels.Memory[l_lineRasterizerIterator.CurrentPoint.x + (l_lineRasterizerIterator.CurrentPoint.y * p_to->Width)] = *p_color;
		}
	};
};


void Draw_PolygonClipped(Polygon2i_PTR p_polygon, Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Material_PTR p_material)
{
	PolygonRasterizerIterator l_rasterizerIterator;
	PolygonRasterize_Initialize(p_polygon, p_clipRect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterize_MoveNext(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			EvaluatePixelColor(p_to, &l_rasterizerIterator.RasterizedPixel, p_material);
		}
	}
};
