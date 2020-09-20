#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"
#include "Objects/Texture/Texture.h"
#include "Heap/RenderHeap_def.h"
#include "v2/_interface/ColorC.h"
#include "v2/_interface/VectorC.h"
#include "Renderer/PixelColor/PixelColorCalculation.h"
#include "Metrics/RenderTimeMetrics.h"

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Color3c_PTR p_color)
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

void Draw_PolygonClipped(PolygonPipelineV2_PTR p_polygonPipeline, Polygon2i_PTR p_polygonPixelPositions, Texture3c_PTR p_to, Recti_PTR p_clipRect, RenderLights_PTR p_renderLights,
	SolidRenderer_Memory_PTR p_solidRendererMemory) {
#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
	size_t tmp_timer_0;
#endif

	Color3f l_pixelColor_3f;
	Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[p_polygonPipeline->Material.Handle];

	PolygonRasterizerIterator l_rasterizerIterator;
	PolygonRasterize_Initialize(p_polygonPixelPositions, p_clipRect, 1, &l_rasterizerIterator);
	POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
	while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
	{

#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
		tmp_timer_0 = Clock_currentTime_mics();
#endif

		l_returnCode = PolygonRasterize_MoveNext(&l_rasterizerIterator);

#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
		PerformanceCounter_AddTime(&GWireframeRendererPerformace.AverageRasterization_PolygonRasterize, Clock_currentTime_mics() - tmp_timer_0);
#endif

		//TODO - Even if the polygon is hided by another one, it will still be shaded and pixel is set to the screen (depth sorting is ensured by polygon ordering)
		// This is something we have to change, as it can causes a lot of pixel calculation wasted.
		// It may be wizer to introduce a BufferTexture that is sampled to ensure that the pixel that we will draw will be visible.
		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{
#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
			tmp_timer_0 = Clock_currentTime_mics();
#endif

			switch (l_material->ShadingType)
			{
			case MATERIAL_SHADING_TYPE_FLAT:
			{
				{
					FlatShadingPixelCalculation_ShadePixelColor(&p_solidRendererMemory->FlatShadingCalculations.Memory[p_polygonPipeline->FlatShadingCalculationIndex], p_polygonPipeline,
						p_renderLights, l_material, &l_rasterizerIterator.InterpolationFactors, &l_pixelColor_3f);
				}
				break;
			}
			case MATERIAL_SHADING_TYPE_NONE:
			{
				{
					l_pixelColor_3f = l_material->BaseColor;
				}
				break;
			}
			break;
			}

			// Interpolation debug display

			// l_pixelColor_3f = (Color3f){ 1.0f, 1.0f, 1.0f };
			// Vec_Mul_3f_1f(&l_pixelColor_3f.Vec, (1.0f * l_rasterizerIterator.I0) , &l_pixelColor_3f.Vec);


			Color_Convert_3F_3C(&l_pixelColor_3f, (Color3c_PTR)&p_to->Pixels.Memory[l_rasterizerIterator.RasterizedPixel.x + (l_rasterizerIterator.RasterizedPixel.y * p_to->Width)]);

#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
			PerformanceCounter_AddTime(&GWireframeRendererPerformace.AverageRasterization_PixelShading, Clock_currentTime_mics() - tmp_timer_0);
#endif

		}

	}

};
