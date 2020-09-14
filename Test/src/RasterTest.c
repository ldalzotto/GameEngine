#include "Renderer/Draw/DrawFunctions.h"
#include "Raster/Rasterizer.h"
#include "v2/_interface/VectorStructuresC.h"

int main()
{
	Polygon2i l_polygon =
	{
		.v1 = {0,0},
		.v2 = {3, 1},
		.v3 = {1, 3}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {100,100} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 100 * 100);

	Rasterize_PolygonClipped(&l_polygon, &l_pixels, &l_rect);
	//Draw_PolygonClipped(&l_polygon, );

	Arr_Clear(&l_pixels.array);
	PolygonRasterizerIterator l_rasterizerIterator;
	PolygonRasterize_Initialize(&l_polygon, &l_rect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterize_MoveNext(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			Arr_PushBackRealloc_Vector2i(&l_pixels, &l_rasterizerIterator.RasterizedPixel);
		}
	}
	return 0;
}