#pragma once

#include "Renderer/Pipeline/RendererPipeline_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "Depth/DepthBuffer_def.h"

typedef struct SolidRendererInput_TYP
{
	RENDEREDOBJECT_BUFFER_PTR RenderableObjectsBuffer;
	CAMERABUFFER_PTR CameraBuffer;
} SolidRendererInput, * SolidRendererInput_PTR;

void DrawObjects_NoShade_NotTextured(const SolidRendererInput* p_input, RenderTexture3f_PTR p_to, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory);
void DrawObjects_FlatShade_Textured_Perspective(const SolidRendererInput* p_input, RenderTexture3f_PTR p_to, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory);
void DrawObjects_FlatShade_NotTextured(const SolidRendererInput* p_input, RenderTexture3f_PTR p_to, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory);