#pragma once

#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "Heap/RenderHeap_def.h"
#include "Renderer/Pipeline/RendererPipeline_def.h"

void RenderableObject_ToRenderPipeline(RENDEREDOBJECT_BUFFER_PTR p_renderableObjects, RendererPipeline_Memory_PTR p_renderPipelineMemory, RenderHeap_PTR p_renderHeap);