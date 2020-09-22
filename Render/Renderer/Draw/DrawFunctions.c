#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"
#include "Objects/Texture/Texture.h"
#include "Heap/RenderHeap_def.h"
#include "v2/_interface/ColorC.h"
#include "v2/_interface/VectorC.h"
#include "Renderer/PixelColor/PixelColorCalculation.h"
#include "Depth/DepthBuffer.h"
#include "Metrics/RenderTimeMetrics.h"

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3f_PTR p_to, Recti_PTR p_clipRect,
	Color3f_PTR p_color)
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

void Draw_PolygonClipped(PolygonPipelineV2_PTR p_polygonPipeline, Polygon2i_PTR p_polygonPixelPositions, Polygonf_PTR p_polygonCameraDepth, Texture3f_PTR p_to,
	Recti_PTR p_clipRect, RenderLights_PTR p_renderLights, DepthBuffer_PTR p_depthBuffer,
	RendererPipeline_Memory_PTR p_solidRendererMemory) {

#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
	size_t tmp_timer_0;
#endif

	Color3f l_pixelColor_3f;

	//TODO -> Avoiding these lookups ? Mesh can be precomputed per object ? Or maybe having a single structure that hold these informations (at least the material and the mesh)
	Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[p_polygonPipeline->Material.Handle];
	Mesh_PTR l_mesh = p_solidRendererMemory->RederableObjectsPipeline.Memory[p_polygonPipeline->AssociatedRenderableObjectPipeline].RenderedObject->Mesh;
	Polygon_UV_PTR l_polygonUV = &RRenderHeap.PolygonUVAllocator.array.Memory[l_mesh->PerVertexData.UV1.Memory[p_polygonPipeline->MeshPolygonIndex].Handle];

	FlatShadingPixelCalculation l_flatCalculation;
	FlatShadingPixelCalculation_PreCalculation(&l_flatCalculation, p_renderLights, p_polygonPipeline);

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

		if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
		{

#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
			tmp_timer_0 = Clock_currentTime_mics();
#endif

			float l_interpolatedDepth = (p_polygonCameraDepth->v1 * l_rasterizerIterator.InterpolationFactors.I0)
				+ (p_polygonCameraDepth->v2 * l_rasterizerIterator.InterpolationFactors.I1) + (p_polygonCameraDepth->v3 * l_rasterizerIterator.InterpolationFactors.I2);

			// Pixel depth culling
			if (DepthBuffer_PushDepthValue(p_depthBuffer, &l_rasterizerIterator.RasterizedPixel, l_interpolatedDepth))
			{
				switch (l_material->ShadingType)
				{
				case MATERIAL_SHADING_TYPE_FLAT:
				{
					{
						FlatShadingPixelCalculation_ShadePixelColor(&l_flatCalculation, l_polygonUV,
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

				p_to->Pixels.Memory[l_rasterizerIterator.RasterizedPixel.x + (l_rasterizerIterator.RasterizedPixel.y * p_to->Width)] = l_pixelColor_3f;

			}




#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
			PerformanceCounter_AddTime(&GWireframeRendererPerformace.AverageRasterization_PixelShading, Clock_currentTime_mics() - tmp_timer_0);
#endif

		}

		}

	};

