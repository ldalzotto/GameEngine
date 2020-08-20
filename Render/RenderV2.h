#pragma once

#include "RenderV2Interface.h"


#include "Renderer/Wireframe/WireframeRenderer.h"
#include "Renderer/Gizmo/Gizmo.h"
#include "Objects/SwapChain/SwapChain_def.h"
#include "Objects/Window/Window.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"



typedef struct GlobalBuffers_TYP
{
	CAMERABUFFER CameraBuffer;
	RENDEREDOBJECT_BUFFER RenderedObjectsBuffer;
}GlobalBuffers, *GlobalBuffers_PTR;

typedef struct RenderResourceProvider_TYP
{
	MESHRESOURCE_PROVIDER MeshResourceProvider;
}RenderResourceProvider, * RenderResourceProvider_PTR;

typedef struct RenderV2_TYP
{
	RenderV2Interface RenderInterface;

	Window AppWindow;
	SwapChain SwapChain;

	RenderResourceProvider Resources;

	GlobalBuffers GlobalBuffer;
	WireframeRenderer_Memory WireframeRenderMemory;
	GizmoBuffer GizmoBuffer;
}RenderV2, * RenderV2_PTR;

void RenderV2_initialize(RenderV2* p_render);
void RenderV2_render(RenderV2* p_render);
void RenderV2_free(RenderV2* p_render);
