#pragma once

#include "Renderer/Pipeline/RendererPipeline_def.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Heap/RenderHeap_def.h"

void RenderableObject_CullObject(RENDEREDOBJECT_BUFFER_PTR p_renderableObjects, RenderHeap_PTR p_renderHeap, CAMERABUFFER_PTR p_cameraBuffer);