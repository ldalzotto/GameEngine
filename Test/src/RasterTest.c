#include "Renderer/Draw/DrawFunctions.h"
#include "Raster/Rasterizer.h"
#include "v2/_interface/VectorStructuresC.h"

typedef struct TestInterp_TYP
{
	float I0, I1, I2;
}TestInterp, * TestInterp_PTR;

typedef struct Array_TestInterp_TYP
{
	ARRAY_TYPE_DEFINITION(TestInterp)
}Array_TestInterp, * Array_TestInterp_PTR;

ARRAY_ALLOC_FUNCTION(TestInterp, Array_TestInterp_PTR, TestInterp)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(TestInterp, Array_TestInterp_PTR, TestInterp);

void DumyRasterizer_Test()
{
	Polygon2i l_polygon =
	{
		.v1 = {1, 0},
		.v2 = {0, 2},
		.v3 = {4, 2}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {319,239} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 100 * 100);
	Array_TestInterp l_interpolations;
	Arr_Alloc_TestInterp(&l_interpolations, 100 * 100);

	//Rasterize_PolygonClipped
	// Rasterize_PolygonClipped(&l_polygon, &l_pixels, &l_rect);
	//Draw_PolygonClipped(&l_polygon, );

	Arr_Clear(&l_pixels.array);
	PolygonRasterizerIterator l_rasterizerIterator;
	PolygonRasterize_Initialize(&l_polygon, &l_rect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterize_MoveNext_Interpolated(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			Arr_PushBackRealloc_Vector2i(&l_pixels, &l_rasterizerIterator.CommonStructure.RasterizedPixel);
			// 
			TestInterp l_interp = {l_rasterizerIterator.CommonStructure.InterpolationFactors.I0, l_rasterizerIterator.CommonStructure.InterpolationFactors.I1, l_rasterizerIterator.CommonStructure.InterpolationFactors.I2 };
			Arr_PushBackRealloc_TestInterp(&l_interpolations, &l_interp);
		}
	}

	int l_debug = 0;
}

void SmartRasterizer_Test()
{
	Polygon2i l_polygon =
	{
		.v1 = {1,0},
		.v2 = {0, 2},
		.v3 = {4, 2}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {319,239} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 100 * 100);
	Array_TestInterp l_interpolations;
	Arr_Alloc_TestInterp(&l_interpolations, 100 * 100);

	//Rasterize_PolygonClipped
	// Rasterize_PolygonClipped(&l_polygon, &l_pixels, &l_rect);
	//Draw_PolygonClipped(&l_polygon, );

	Arr_Clear(&l_pixels.array);
	PolygonRasterizeSmartIterator l_rasterizerIterator;
	PolygonRasterizeSmart_Initialize(&l_polygon, &l_rect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterizeSmart_MoveNext_Interpolated(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			Arr_PushBackRealloc_Vector2i(&l_pixels, &l_rasterizerIterator.CommonStructure.RasterizedPixel);

			// TestInterp l_interp = {l_rasterizerIterator.I0, l_rasterizerIterator.I1, l_rasterizerIterator.I2 };
			// Arr_PushBackRealloc_TestInterp(&l_interpolations, &l_interp);
		}
	}
	int l_debug = 0;
}

int main()
{
	DumyRasterizer_Test();
	SmartRasterizer_Test();
	return 0;
}