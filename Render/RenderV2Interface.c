#include "RenderV2Interface.h"

#include "RenderV2.h"

void RenderV2Interface_build(RenderV2Interface* p_renderInterface, RenderV2* p_render)
{
	p_renderInterface->AppWindow = &p_render->AppWindow;
	p_renderInterface->SwapChain = &p_render->SwapChain;
	p_renderInterface->Resources.MeshResourceProvider = &p_render->Resources.MeshResourceProvider;
	p_renderInterface->GlobalBuffer.CameraBuffer = &p_render->GlobalBuffer.CameraBuffer;
	p_renderInterface->GlobalBuffer.RenderedObjectsBuffer = &p_render->GlobalBuffer.RenderedObjectBuffers;
	p_renderInterface->WireframeRenderMemory = &p_render->WireframeRenderMemory;
	p_renderInterface->GizmoBuffer = &p_render->GizmoBuffer;
};
