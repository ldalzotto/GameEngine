#include "PixelColorCalculation.h"
#include "Heap/RenderHeap_def.h"
#include <math.h>
#include "Constants.h"
#include "v2/_interface/VectorC.h"

void PixelColorCaluclation_Polygon_PushCalculations(PolygonPipelineV2_PTR p_polygonPipeline, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[p_polygonPipeline->Material.Handle];
	switch (l_material->ShadingType)
	{
	case MATERIAL_SHADING_TYPE_FLAT:
	{
		Arr_PushBackRealloc_Empty_FlatShadingPixelCalculation(&p_solidRendererMemory->FlatShadingCalculations);
		p_solidRendererMemory->FlatShadingCalculations.Memory[p_solidRendererMemory->FlatShadingCalculations.Size - 1] = (FlatShadingPixelCalculation)
		{
			.PolygonVaryings = p_polygonPipeline->PolygonVaryingIndex,
			.Out_DotProduct = 0.0f
		};

		p_polygonPipeline->FlatShadingCalculationIndex = p_solidRendererMemory->FlatShadingCalculations.Size - 1;
		
	}
	break;
	}
};

void FlatShadingPixelCalculation_Calculate(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, DirectionalLight_PTR p_directionalLight, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	PolygonVaryings_PTR l_polygonVaryings = &p_solidRendererMemory->PolygonVaryings.Memory[p_flatShadingPixelCalculation->PolygonVaryings];
	p_flatShadingPixelCalculation->Out_DotProduct = Vec_Dot_3f(&p_directionalLight->Direction, &l_polygonVaryings->WorldFlatNormal.Vec3) * p_directionalLight->Intensity;
	if (p_flatShadingPixelCalculation->Out_DotProduct <= FLOAT_TOLERANCE) { p_flatShadingPixelCalculation->Out_DotProduct = 0.0f; }
};