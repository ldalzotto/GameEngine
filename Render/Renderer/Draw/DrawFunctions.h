#pragma once

#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/ColorC_def.h"
#include "v2/_interface/VectorStructuresC_def.h"
#include "Objects/Resource/Polygon_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Objects/Resource/Material_def.h"
#include "Renderer/Solid/SolidRenderer.h"
#include "Light/Light_def.h"

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Color3c_PTR p_color);

void Draw_PolygonClipped(PolygonPipelineV2_PTR p_polygonPipeline, Polygon2i_PTR p_polygonPixelPositions, Texture3c_PTR p_to, Recti_PTR p_clipRect, RenderLights_PTR p_renderLights,
		SolidRenderer_Memory_PTR p_solidRendererMemory);