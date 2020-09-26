#pragma once

#include "Renderer/Pipeline/RendererPipeline_def.h"
#include "Heap/RenderHeap_def.h"
#include "Depth/DepthBuffer_def.h"
#include "Objects/Texture/Texture_def.h"
#include "v2/_interface/RectC_def.h"
#include "Light/Light_def.h"

typedef struct DrawPolygFlatShadeTexturedInput_TYP
{
	RendererPipeline_Memory_PTR RendererPipelineMemory;
	RenderHeap_PTR RenderHeap;
	RenderTexture3f_PTR RenderTarget;
	DepthBuffer_PTR DepthBuffer;
	RenderLights_PTR RenderLights;
}DrawPolygFlatShadeTexturedInput, * DrawPolygFlatShadeTexturedInput_PTR;

void DrawPoly_NoShade_NotTextured(DrawPolygFlatShadeTexturedInput_PTR p_input);
void DrawPoly_FlatShade_Textured_Perspective(DrawPolygFlatShadeTexturedInput_PTR p_input);
void DrawPoly_FlatShade_NotTextured(DrawPolygFlatShadeTexturedInput_PTR p_input);