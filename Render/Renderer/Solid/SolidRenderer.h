#pragma once

#include "Objects/RenderedObject.h"
#include "Renderer/Pipeline/RendererPipeline_def.h"
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/WindowSize_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "Depth/DepthBuffer_def.h"

typedef struct SolidRendererInput_TYP
{
	RENDEREDOBJECT_BUFFER_PTR RenderableObjectsBuffer;
	CAMERABUFFER_PTR CameraBuffer;
	WindowSize WindowSize;
} SolidRendererInput, * SolidRendererInput_PTR;



void SolidRenderer_Memory_alloc(RendererPipeline_Memory_PTR p_memory);
void SolidRenderer_Memory_clear(RendererPipeline_Memory_PTR p_memory, size_t p_width, size_t height);
void SolidRenderer_Memory_free(RendererPipeline_Memory_PTR p_memory);

void SolidRenderer_renderV2(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory);
