#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"
#include "v2/_interface/VectorC.h"
#include "Objects/Texture/Texture.h"
#include "Heap/RenderHeap_def.h"
#include "v2/_interface/ColorC.h"
#include "Renderer/PixelColor/PixelColorCalculation.h"

#include "Metrics/RenderTimeMetrics.h"

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

void Draw_PolygonClipped(PolygonPipelineV2_PTR p_polygonPipeline, Polygon2i_PTR p_polygonPixelPositions, Texture3c_PTR p_to, Recti_PTR p_clipRect, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
#if RENDER_PERFORMANCE_TIMER
	size_t tmp_timer_0;
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer_0 = Clock_currentTime_mics();
#endif

	Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[p_polygonPipeline->Material.Handle];
	Color3c l_pixelColor;

	switch (l_material->ShadingType)
	{
	case MATERIAL_SHADING_TYPE_FLAT:
	{
		Color3f l_pixelColorF;
		FlatShadingPixelCalculation_ShadeColor(&p_solidRendererMemory->FlatShadingCalculations.Memory[p_polygonPipeline->FlatShadingCalculationIndex], &l_material->BaseColor, &l_pixelColorF);
		Color_Convert_3F_3C(&l_pixelColorF, &l_pixelColor);
	}
	break;
	case MATERIAL_SHADING_TYPE_NONE:
	{
		Color_Convert_3F_3C(&l_material->BaseColor, &l_pixelColor);
	}
	break;
	default:
	{
		l_pixelColor = (Color3c) { 0 };
	}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_AddTime(&GWireframeRendererPerformace.AverageRasterization_PixelLight, Clock_currentTime_mics() - tmp_timer_0);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer_0 = Clock_currentTime_mics();
#endif

	PolygonRasterizerIterator l_rasterizerIterator;
	PolygonRasterize_Initialize(p_polygonPixelPositions, p_clipRect, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{
		l_returnCode = PolygonRasterize_MoveNext(&l_rasterizerIterator);
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
			p_to->Pixels.Memory[l_rasterizerIterator.RasterizedPixel.x + (l_rasterizerIterator.RasterizedPixel.y * p_to->Width)] = l_pixelColor.Vec;
		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_AddTime(&GWireframeRendererPerformace.AverageRasterization_PixelRasterize, Clock_currentTime_mics() - tmp_timer_0);
#endif

};
