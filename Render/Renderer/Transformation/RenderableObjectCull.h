#pragma once

#include "Renderer/Pipeline/RendererPipeline_def.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"

void RenderableObject_CullObject(RENDEREDOBJECT_BUFFER_PTR p_renderableObjects, CAMERABUFFER_PTR p_cameraBuffer);