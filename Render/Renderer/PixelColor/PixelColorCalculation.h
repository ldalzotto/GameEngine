#pragma once

#include "PixelColorCalculation_def.h"
#include "DataStructures/ARRAY.h"
#include "Renderer/Solid/SolidRenderer.h"
#include "Light/Light_def.h"
#include "Raster/Rasterizer.h"

ARRAY_ALLOC_FUNCTION(FlatShadingPixelCalculation, Array_FlatShadingPixelCalculation_PTR, FlatShadingPixelCalculation)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(FlatShadingPixelCalculation, Array_FlatShadingPixelCalculation_PTR, FlatShadingPixelCalculation)

void FlatShadingPixelCalculation_PreCalculation(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, RenderLights_PTR p_renderLights, PolygonPipelineV2_PTR p_polygonPipeline);

void FlatShadingPixelCalculation_ShadePixelColor(
	const FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, const Polygon2f_PTR p_polygonUV,
	RenderLights_PTR p_renderLights, Material_PTR p_material,
	const PolygonRasterizer_InterpolationFactor_PTR p_interpolationFactors, Color3f_PTR out_pixelColor);

// void FlatShadingPixelcalculation_PixelColor()