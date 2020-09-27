#include "Renderer/Draw/DrawFunctions.h"
#include "Raster/Rasterizer.h"
#include "v2/_interface/VectorStructuresC.h"
#include "Lib/Assertion.h"
#include "Functional/Equals/Equals.h"

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

inline void Assert_Contains_TestInterpArr_Ordered(Array_TestInterp_PTR p_vec, Array_TestInterp_PTR p_item, const char* p_message)
{
	for (size_t i = 0; i < p_item->Size; i++)
	{
		if (!Equals_float(&p_vec->Memory[i].I0, &p_item->Memory[i].I0)
			|| !Equals_float(&p_vec->Memory[i].I1, &p_item->Memory[i].I1)
			|| !Equals_float(&p_vec->Memory[i].I2, &p_item->Memory[i].I2))
		{
			if (p_message) { printf(p_message); }
			abort();
		}

	}
}

void SmartRasterizer_NoSingularity_Test()
{
	Polygon2i l_polygon =
	{
		.v1 = {0, 1},
		.v2 = {5, 0},
		.v3 = {2, 3}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {10,10} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 10 * 10);
	Array_TestInterp l_interpolations;
	Arr_Alloc_TestInterp(&l_interpolations, 10 * 10);
	Arr_Clear(&l_pixels.array);
	Arr_Clear(&l_interpolations.array);

	PolygonRasterizeSmartIterator l_rasterizerIterator;
	PolygonRasterizeSmart_Initialize(&l_polygon, &l_rect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterizeSmart_MoveNext_Interpolated(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			Arr_PushBackRealloc_Vector2i(&l_pixels, &l_rasterizerIterator.CommonStructure.RasterizedPixel);

			TestInterp l_interp = { l_rasterizerIterator.CommonStructure.InterpolationFactors.I0, l_rasterizerIterator.CommonStructure.InterpolationFactors.I1, l_rasterizerIterator.CommonStructure.InterpolationFactors.I2 };
			Arr_PushBackRealloc_TestInterp(&l_interpolations, &l_interp);
		}
	}

	Vector2i l_awaitedPixels[10];
	short int i = 0;
	l_awaitedPixels[i++] = (Vector2i){ 5, 0 };
	l_awaitedPixels[i++] = (Vector2i){ 0, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 1, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 2, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 3, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 4, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 1, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 2, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 3, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 2, 3 };
	Array_Vector2i l_awaitedPixels_arr = { .Memory = l_awaitedPixels, .Size = 10 };

	TestInterp l_awaitedInterpolators[10];
	i = 0;
	l_awaitedInterpolators[i++] = (TestInterp){ 0.0f, 0.0f, 1.0f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.25f, 0.166666672f, 0.583333313f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.5f, 0.00f, 0.50f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.00f, 0.333333343f, 0.666666687f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.50f, 0.333333343f, 0.166666672f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.75f, 0.166666672f, 0.0833333358f };
	l_awaitedInterpolators[i++] = (TestInterp){ 1.0f, 0.00f, 0.00f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.25f, 0.5f, 0.25f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.0f, 0.666666687f, 0.333333343f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.0f, 1.0f, 0.0f };
	Array_TestInterp l_awaitedInterpolators_arr = { .Memory = l_awaitedInterpolators, .Size = 10 };

	Assert_Equals_Sizet(l_pixels.Size, 10, NULL);
	Assert_Contains_Vector2iArr(&l_pixels, &l_awaitedPixels_arr, NULL);
	Assert_Contains_TestInterpArr_Ordered(&l_interpolations, &l_awaitedInterpolators_arr, NULL);
	
	Arr_Free(&l_pixels.array);
	Arr_Free(&l_interpolations.array);
}

void SmartRasterizer_TwoUpVertexSameY_Test()
{
	Polygon2i l_polygon =
	{
		.v1 = {1, 0},
		.v2 = {0, 2},
		.v3 = {4, 2}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {10,10} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 10 * 10);
	Array_TestInterp l_interpolations;
	Arr_Alloc_TestInterp(&l_interpolations, 10 * 10);
	Arr_Clear(&l_pixels.array);
	Arr_Clear(&l_interpolations.array);

	PolygonRasterizeSmartIterator l_rasterizerIterator;
	PolygonRasterizeSmart_Initialize(&l_polygon, &l_rect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterizeSmart_MoveNext_Interpolated(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			Arr_PushBackRealloc_Vector2i(&l_pixels, &l_rasterizerIterator.CommonStructure.RasterizedPixel);

			TestInterp l_interp = { l_rasterizerIterator.CommonStructure.InterpolationFactors.I0, l_rasterizerIterator.CommonStructure.InterpolationFactors.I1, l_rasterizerIterator.CommonStructure.InterpolationFactors.I2 };
			Arr_PushBackRealloc_TestInterp(&l_interpolations, &l_interp);
		}
	}

	Vector2i l_awaitedPixels[8];
	short int i = 0;
	l_awaitedPixels[i++] = (Vector2i){ 1, 0 };
	l_awaitedPixels[i++] = (Vector2i){ 1, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 2, 1 };
	l_awaitedPixels[i++] = (Vector2i){ 0, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 1, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 2, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 3, 2 };
	l_awaitedPixels[i++] = (Vector2i){ 4, 2 };
	Array_Vector2i l_awaitedPixels_arr = { .Memory = l_awaitedPixels, .Size = 8 };

	TestInterp l_awaitedInterpolators[8];
	i = 0;
	l_awaitedInterpolators[i++] = (TestInterp){ 0.0f, 0.0f, 1.0f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.0f, 0.25f, 0.75f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.0f, 0.5f, 0.5f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.00f, 0.75f, 0.25f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.00f, 1.0f, 0.0f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.5f, 0.125f, 0.375f };
	l_awaitedInterpolators[i++] = (TestInterp){ 0.5f, 0.375f, 0.125f };
	l_awaitedInterpolators[i++] = (TestInterp){ 1.0f, 0.0f, 0.0f };
	Array_TestInterp l_awaitedInterpolators_arr = { .Memory = l_awaitedInterpolators, .Size = 8 };

	Assert_Equals_Sizet(l_pixels.Size, 8, NULL);
	Assert_Contains_Vector2iArr(&l_pixels, &l_awaitedPixels_arr, NULL);
	Assert_Contains_TestInterpArr_Ordered(&l_interpolations, &l_awaitedInterpolators_arr, NULL);
	
	Arr_Free(&l_pixels.array);
	Arr_Free(&l_interpolations.array);
}

void SmartRasterizer_VertexSamePosition_Test()
{
	Polygon2i l_polygon =
	{
		.v1 = {1, 0},
		.v2 = {1, 0},
		.v3 = {4, 2}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {10,10} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 10 * 10);
	Array_TestInterp l_interpolations;
	Arr_Alloc_TestInterp(&l_interpolations, 10 * 10);
	Arr_Clear(&l_pixels.array);
	Arr_Clear(&l_interpolations.array);

	PolygonRasterizeSmartIterator l_rasterizerIterator;
	PolygonRasterizeSmart_Initialize(&l_polygon, &l_rect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterizeSmart_MoveNext_Interpolated(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			Arr_PushBackRealloc_Vector2i(&l_pixels, &l_rasterizerIterator.CommonStructure.RasterizedPixel);

			TestInterp l_interp = { l_rasterizerIterator.CommonStructure.InterpolationFactors.I0, l_rasterizerIterator.CommonStructure.InterpolationFactors.I1, l_rasterizerIterator.CommonStructure.InterpolationFactors.I2 };
			Arr_PushBackRealloc_TestInterp(&l_interpolations, &l_interp);
		}
	}


	Assert_Equals_Sizet(l_pixels.Size, 0, NULL);

	Arr_Free(&l_pixels.array);
	Arr_Free(&l_interpolations.array);
}

int main()
{
	SmartRasterizer_NoSingularity_Test();
	SmartRasterizer_TwoUpVertexSameY_Test();
	SmartRasterizer_VertexSamePosition_Test();
	return 0;
}