#include "FlatShade.h"
#include "Raster/Rasterizer.h"
#include "Depth/DepthBuffer.h"
#include "v2/_interface/ColorC_def.h"
#include "v2/_interface/VectorC.h"
#include "Objects/Resource/Polygon_inline.c"
#include "Objects/Texture/TextureSampling_inline.c"
#include "Constants.h"

typedef struct DrawFunction_ExtractedPipeline_TYp
{
	PolygonPipelineV2_PTR Polygon;
	RenderableObjectPipeline_PTR RenderableObjectPipeline;
	VertexPipeline_PTR V1;
	VertexPipeline_PTR V2;
	VertexPipeline_PTR V3;
}DrawFunction_ExtractedPipeline, * DrawFunction_ExtractedPipeline_PTR;

inline DrawFunction_ExtractedPipeline _i_ExtractPipeline(RendererPipeline_Memory_PTR p_renderPipelineMemory, size_t p_polygonIndex)
{
	DrawFunction_ExtractedPipeline l_extranctedData;
	l_extranctedData.Polygon = &p_renderPipelineMemory->PolygonPipelines.Memory[p_renderPipelineMemory->OrderedPolygonPipelinesIndex.Memory[p_polygonIndex].Index];
	l_extranctedData.RenderableObjectPipeline = &p_renderPipelineMemory->RederableObjectsPipeline.Memory[l_extranctedData.Polygon->AssociatedRenderableObjectPipeline];

	l_extranctedData.V1 = &p_renderPipelineMemory->VertexPipeline.Memory[l_extranctedData.Polygon->VerticesPipelineIndex.v1];
	l_extranctedData.V2 = &p_renderPipelineMemory->VertexPipeline.Memory[l_extranctedData.Polygon->VerticesPipelineIndex.v2];
	l_extranctedData.V3 = &p_renderPipelineMemory->VertexPipeline.Memory[l_extranctedData.Polygon->VerticesPipelineIndex.v3];

	return l_extranctedData;
};

inline Polygon2i _i_ExtractedPipeline_PixelPositionPolygon(DrawFunction_ExtractedPipeline_PTR p_extractedPipeline)
{
	return (Polygon2i)
	{
		.v1 = p_extractedPipeline->V1->PixelPosition,
		.v2 = p_extractedPipeline->V2->PixelPosition,
		.v3 = p_extractedPipeline->V3->PixelPosition
	};
};
inline Polygonf _i_ExtractedPipeline_CameraDepthPolygon(DrawFunction_ExtractedPipeline_PTR p_extractedPipeline)
{
	return (Polygonf)
	{
		.v1 = p_extractedPipeline->V1->CameraSpacePosition.z,
		.v2 = p_extractedPipeline->V2->CameraSpacePosition.z,
		.v3 = p_extractedPipeline->V3->CameraSpacePosition.z
	};
};


inline char _i_DepthTest(PolygonRasterizerIterator_PTR p_rasterizer, Polygonf_PTR p_cameraDepthPolygon, DepthBuffer_PTR p_depthBuffer)
{
	float l_interpolatedDepth = (p_cameraDepthPolygon->v1 * p_rasterizer->InterpolationFactors.I0)
		+ (p_cameraDepthPolygon->v2 * p_rasterizer->InterpolationFactors.I1) + (p_cameraDepthPolygon->v3 * p_rasterizer->InterpolationFactors.I2);

	// Pixel depth culling
	return DepthBuffer_PushDepthValue(p_depthBuffer, &p_rasterizer->RasterizedPixel, l_interpolatedDepth);
}

typedef struct FlatShadingPixelCalculation_TYP
{
	Color3f AttenuatedLightColor;
}FlatShadingPixelCalculation, * FlatShadingPixelCalculation_PTR;

inline void _i_FlatShadingPixelCalculation_PreCalculation(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, RenderLights_PTR p_renderLights, PolygonPipelineV2_PTR p_polygonPipeline)
{
	float l_attenuation = Vec_Dot_3f(&p_renderLights->DirectionalLight.Direction, &p_polygonPipeline->CalculatedFlatWorldNormal) * p_renderLights->DirectionalLight.Intensity;
	if (l_attenuation <= FLOAT_TOLERANCE) { l_attenuation = 0.0f; }

	Vec_Mul_3f_1f(&p_renderLights->DirectionalLight.Color.Vec, l_attenuation, &p_flatShadingPixelCalculation->AttenuatedLightColor.Vec);
};

inline void _i_FlatShadingPixelCalculation_ShadePixelColor(const FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, 
		RenderLights_PTR p_renderLights, Material_PTR p_material, Color3f_PTR out_pixelColor)
{
	out_pixelColor->Vec.x = (p_material->BaseColor.r) * (p_flatShadingPixelCalculation->AttenuatedLightColor.r + p_renderLights->AmbientLight.Color.r);
	out_pixelColor->Vec.y = (p_material->BaseColor.g) * (p_flatShadingPixelCalculation->AttenuatedLightColor.g + p_renderLights->AmbientLight.Color.g);
	out_pixelColor->Vec.z = (p_material->BaseColor.b) * (p_flatShadingPixelCalculation->AttenuatedLightColor.b + p_renderLights->AmbientLight.Color.b);
}

inline void _i_FlatShadingPixelCalculation_ShadePixelColor_Textured(
	const FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, const Polygon2f_PTR p_polygonUV,
	RenderLights_PTR p_renderLights, Material_PTR p_material,
	const PolygonRasterizer_InterpolationFactor_PTR p_interpolationFactors, Color3f_PTR out_pixelColor)
{

	Vector2f l_interpolatedUv;
	Color3f l_sampledPoint;

	_i_Polygon_Interpolate_V2F(p_polygonUV, p_interpolationFactors->I0, p_interpolationFactors->I1, p_interpolationFactors->I2, &l_interpolatedUv);
	_i_TextureSample_Point_3f(&RRenderHeap.Texture3cAllocator.array.Memory[p_material->DiffuseTexture.Handle], &l_interpolatedUv, &l_sampledPoint);

	out_pixelColor->Vec.x = (p_material->BaseColor.r * l_sampledPoint.r) * (p_flatShadingPixelCalculation->AttenuatedLightColor.r + p_renderLights->AmbientLight.Color.r);
	out_pixelColor->Vec.y = (p_material->BaseColor.g * l_sampledPoint.g) * (p_flatShadingPixelCalculation->AttenuatedLightColor.g + p_renderLights->AmbientLight.Color.g);
	out_pixelColor->Vec.z = (p_material->BaseColor.b * l_sampledPoint.b) * (p_flatShadingPixelCalculation->AttenuatedLightColor.b + p_renderLights->AmbientLight.Color.b);

};

void DrawPoly_NoShade_NotTextured(DrawPolygFlatShadeTexturedInput_PTR p_input)
{
	for (size_t i = 0; i < p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Size; i++)
	{
		DrawFunction_ExtractedPipeline l_pipelineData = _i_ExtractPipeline(p_input->RendererPipelineMemory, i);

		Material_PTR l_material = &p_input->RenderHeap->MaterialAllocator.array.Memory[l_pipelineData.RenderableObjectPipeline->RenderedObject->Material.Handle];
		Polygon_UV_PTR l_polygonUV = &p_input->RenderHeap->PolygonUVAllocator.array.Memory
			[l_pipelineData.RenderableObjectPipeline->RenderedObject->Mesh->PerVertexData.UV1.Memory
			[l_pipelineData.Polygon->MeshPolygonIndex].Handle];

		// Rasterize
		Polygon2i l_pixelPositionPolygon = _i_ExtractedPipeline_PixelPositionPolygon(&l_pipelineData);
		Polygonf l_cameraDepthPolygon = _i_ExtractedPipeline_CameraDepthPolygon(&l_pipelineData);

		PolygonRasterizerIterator l_rasterizerIterator;
		PolygonRasterize_Initialize(&l_pixelPositionPolygon, p_input->TargetClip, &l_rasterizerIterator);

		POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
		{
			l_returnCode = PolygonRasterize_MoveNext_Interpolated(&l_rasterizerIterator);

			if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
			{
				if (_i_DepthTest(&l_rasterizerIterator, &l_cameraDepthPolygon, p_input->DepthBuffer))
				{
					p_input->RenderTarget->Pixels.Memory[l_rasterizerIterator.RasterizedPixel.x + (l_rasterizerIterator.RasterizedPixel.y * p_input->RenderTarget->Width)] = l_material->BaseColor;
				}
			}
		}


	}
};


void DrawPoly_FlatShade_Textured(DrawPolygFlatShadeTexturedInput_PTR p_input)
{
	Color3f l_pixelColor;

	for (size_t i = 0; i < p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Size; i++)
	{
		DrawFunction_ExtractedPipeline l_pipelineData = _i_ExtractPipeline(p_input->RendererPipelineMemory, i);

		Material_PTR l_material = &p_input->RenderHeap->MaterialAllocator.array.Memory[l_pipelineData.RenderableObjectPipeline->RenderedObject->Material.Handle];
		Polygon_UV_PTR l_polygonUV = &p_input->RenderHeap->PolygonUVAllocator.array.Memory
				[l_pipelineData.RenderableObjectPipeline->RenderedObject->Mesh->PerVertexData.UV1.Memory
						[l_pipelineData.Polygon->MeshPolygonIndex].Handle];


		FlatShadingPixelCalculation l_flatCalculation;
		_i_FlatShadingPixelCalculation_PreCalculation(&l_flatCalculation, p_input->RenderLights, l_pipelineData.Polygon);

		// Rasterize
		Polygon2i l_pixelPositionPolygon = _i_ExtractedPipeline_PixelPositionPolygon(&l_pipelineData);
		Polygonf l_cameraDepthPolygon = _i_ExtractedPipeline_CameraDepthPolygon(&l_pipelineData);

		PolygonRasterizerIterator l_rasterizerIterator;
		PolygonRasterize_Initialize(&l_pixelPositionPolygon, p_input->TargetClip, &l_rasterizerIterator);

		POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
		{
			l_returnCode = PolygonRasterize_MoveNext_Interpolated(&l_rasterizerIterator);

			if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
			{
				if (_i_DepthTest(&l_rasterizerIterator, &l_cameraDepthPolygon, p_input->DepthBuffer))
				{
					_i_FlatShadingPixelCalculation_ShadePixelColor_Textured(&l_flatCalculation, l_polygonUV, p_input->RenderLights, l_material, &l_rasterizerIterator.InterpolationFactors, &l_pixelColor);
					p_input->RenderTarget->Pixels.Memory[l_rasterizerIterator.RasterizedPixel.x + (l_rasterizerIterator.RasterizedPixel.y * p_input->RenderTarget->Width)] = l_pixelColor;
				}
			}
		}


	}
};

void DrawPoly_FlatShade_NotTextured(DrawPolygFlatShadeTexturedInput_PTR p_input)
{
	Color3f l_pixelColor;

	for (size_t i = 0; i < p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Size; i++)
	{
		DrawFunction_ExtractedPipeline l_pipelineData = _i_ExtractPipeline(p_input->RendererPipelineMemory, i);

		Material_PTR l_material = &p_input->RenderHeap->MaterialAllocator.array.Memory[l_pipelineData.RenderableObjectPipeline->RenderedObject->Material.Handle];

		FlatShadingPixelCalculation l_flatCalculation;
		_i_FlatShadingPixelCalculation_PreCalculation(&l_flatCalculation, p_input->RenderLights, l_pipelineData.Polygon);

		// Rasterize
		Polygon2i l_pixelPositionPolygon = _i_ExtractedPipeline_PixelPositionPolygon(&l_pipelineData);
		Polygonf l_cameraDepthPolygon = _i_ExtractedPipeline_CameraDepthPolygon(&l_pipelineData);

		PolygonRasterizerIterator l_rasterizerIterator;
		PolygonRasterize_Initialize(&l_pixelPositionPolygon, p_input->TargetClip, &l_rasterizerIterator);

		POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
		while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
		{
			l_returnCode = PolygonRasterize_MoveNext_Interpolated(&l_rasterizerIterator);

			if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
			{
				if (_i_DepthTest(&l_rasterizerIterator, &l_cameraDepthPolygon, p_input->DepthBuffer))
				{
					_i_FlatShadingPixelCalculation_ShadePixelColor(&l_flatCalculation, p_input->RenderLights, l_material, &l_pixelColor);
					p_input->RenderTarget->Pixels.Memory[l_rasterizerIterator.RasterizedPixel.x + (l_rasterizerIterator.RasterizedPixel.y * p_input->RenderTarget->Width)] = l_pixelColor;
				}
			}
		}


	}
};