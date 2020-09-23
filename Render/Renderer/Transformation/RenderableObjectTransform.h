#pragma once

#include "Renderer/Pipeline/RendererPipeline_def.h"
#include "Heap/RenderHeap_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "v2/_interface/WindowSize.h"
#include "Renderer/OpenCL/OpenCLContext_def.h"

typedef struct RenderableObjectTransform_Input_TYP
{
	RendererPipeline_Memory_PTR RendererPipelineMemory;
	RenderHeap_PTR RenderHeap;
	CAMERABUFFER_PTR CameraBuffer;
	WindowSize_PTR WindowSize;
}RenderableObjectTransform_Input, * RenderableObjectTransform_Input_PTR;

void RendereableObject_TransformPolygons(RenderableObjectTransform_Input_PTR p_input);

#if HAS_OPENCL
void RendereableObject_TransformPolygons_vertexCentralized_GPU(RenderableObjectTransform_Input_PTR p_input, OpenCLContext_PTR p_openCLContext);
#endif