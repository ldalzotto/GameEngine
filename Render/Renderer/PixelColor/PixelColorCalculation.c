#include "PixelColorCalculation.h"
#include "Heap/RenderHeap_def.h"
#include <math.h>
#include "Constants.h"
#include "Objects/Resource/Polygon.h"
#include "Objects/Texture/TextureSampling.h"
#include "v2/_interface/VectorC.h"

void PixelColorCaluclation_Polygon_PushCalculations(PolygonPipelineV2_PTR p_polygonPipeline, Vector4f_PTR p_polygonWorldNormal, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[p_polygonPipeline->Material.Handle];
	switch (l_material->ShadingType)
	{
	case MATERIAL_SHADING_TYPE_FLAT:
	{
		Arr_PushBackRealloc_Empty_FlatShadingPixelCalculation(&p_solidRendererMemory->FlatShadingCalculations);
		p_solidRendererMemory->FlatShadingCalculations.Memory[p_solidRendererMemory->FlatShadingCalculations.Size - 1] = (FlatShadingPixelCalculation)
		{
			.PolygonFlatNormal = *p_polygonWorldNormal,
			.AttenuatedLightColor = (Color3f){0.0f, 0.0f, 0.0f}
		};

		p_polygonPipeline->FlatShadingCalculationIndex = p_solidRendererMemory->FlatShadingCalculations.Size - 1;
		
	}
	break;
	}
};

void FlatShadingPixelCalculation_PreCalculation(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, RenderLights_PTR p_renderLights, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	float l_attenuation = Vec_Dot_3f(&p_renderLights->DirectionalLight.Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) * p_renderLights->DirectionalLight.Intensity;
	// float l_attenuation = (Vec_Dot_3f(&p_renderLights->DirectionalLight.Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) + 1.0f) * 0.5f * p_renderLights->DirectionalLight.Intensity;
	if (l_attenuation <= FLOAT_TOLERANCE) { l_attenuation = 0.0f; }

	Vec_Mul_3f_1f(&p_renderLights->DirectionalLight.Color.Vec, l_attenuation, &p_flatShadingPixelCalculation->AttenuatedLightColor.Vec);
};

void FlatShadingPixelCalculation_ShadePixelColor(
	const FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, const PolygonPipelineV2_PTR p_polygonPipeline, 
	RenderLights_PTR p_renderLights, Material_PTR p_material,
	const PolygonRasterizerIterator_PTR p_polygonRasterize, Color3f_PTR out_pixelColor)
{
	
	switch (p_material->MeshPropertyUsage)
	{
	case MATERIAL_MESHPROPERTY_USAGE_UV:
	{
		Vector2f l_interpolatedUv;
		Polygon_Interpolate_V2F(&RRenderHeap.PolygonUVAllocator.array.Memory[p_polygonPipeline->MaterialMeshProperties.PolygonUV.Handle],
			p_polygonRasterize->I0, p_polygonRasterize->I1, p_polygonRasterize->I2, &l_interpolatedUv);

		Color3f l_sampledPoint;
		TextureSample_Point_3f(&RRenderHeap.Texture3cAllocator.array.Memory[p_material->DiffuseTexture.Handle], &l_interpolatedUv, &l_sampledPoint);

		// *out_pixelColor = (Color3f){ l_interpolatedUv.x, l_interpolatedUv.y, 0.0f };
		// *out_pixelColor = l_sampledPoint;
		Vec_Mul_3f_3f(&p_material->BaseColor.Vec, &l_sampledPoint.Vec, &out_pixelColor->Vec);
		Vec_Mul_3f_3f(&out_pixelColor->Vec, &p_flatShadingPixelCalculation->AttenuatedLightColor.Vec, &out_pixelColor->Vec);
		Vec_Add_3f_3f(&out_pixelColor->Vec, &p_renderLights->AmbientLight.Color.Vec, &out_pixelColor->Vec);
	}
		break;
	default:
	{
		Vec_Mul_3f_3f(&p_material->BaseColor.Vec, &p_flatShadingPixelCalculation->AttenuatedLightColor.Vec, &out_pixelColor->Vec);
		Vec_Add_3f_3f(&out_pixelColor->Vec, &p_renderLights->AmbientLight.Color.Vec, &out_pixelColor->Vec);
	}
		break;
	}
	
};