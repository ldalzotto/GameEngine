#include "PixelColorCalculation.h"
#include "Heap/RenderHeap_def.h"
#include <math.h>
#include "Constants.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/ColorC.h"
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
			.Out_AttenuatedLightColor = (Color3f){0.0f, 0.0f, 0.0f}
		};

		p_polygonPipeline->FlatShadingCalculationIndex = p_solidRendererMemory->FlatShadingCalculations.Size - 1;
		
	}
	break;
	}
};

void FlatShadingPixelCalculation_Calculate(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, RenderLights_PTR p_renderLights, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	float l_attenuation = Vec_Dot_3f(&p_renderLights->DirectionalLight.Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) * p_renderLights->DirectionalLight.Intensity;
	if (l_attenuation <= FLOAT_TOLERANCE) { l_attenuation = 0.0f; }

	Vec_Mul_3f_1f(&p_renderLights->DirectionalLight.Color.Vec, l_attenuation, &p_flatShadingPixelCalculation->Out_AttenuatedLightColor.Vec);
};

//We can have Polygon here
void FlatShadingPixelCalculation_ShadePolygonColor(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, RenderLights_PTR p_renderLights, Color3f_PTR p_color, Color3f_PTR out_color)
{
	Vec_Mul_3f_3f(&p_color->Vec, &p_flatShadingPixelCalculation->Out_AttenuatedLightColor.Vec, &out_color->Vec);
	Vec_Add_3f_3f(&out_color->Vec, &p_renderLights->AmbientLight.Color.Vec, &out_color->Vec);
};

void FlatShadingPixelCalculation_ShadePixelColor(const PolygonPipelineV2_PTR p_polygonPipeline, 
	const PolygonRasterizerIterator_PTR p_polygonRasterize,
	const Color3c_PTR p_polygonColor, Color3c_PTR out_pixelColor)
{
	Vector2f l_interpolatedUv;
	Polygon_Interpolate_V2F(&RRenderHeap.PolygonUVAllocator.array.Memory[p_polygonPipeline->MaterialMeshProperties.PolygonUV.Handle], 
		p_polygonRasterize->I0, p_polygonRasterize->I1, p_polygonRasterize->I2, &l_interpolatedUv);
	Color3f l_color = { l_interpolatedUv.x, l_interpolatedUv.y, 0.0f };
	Color_Convert_3F_3C(&l_color, out_pixelColor);
};