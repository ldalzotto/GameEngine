#pragma once

typedef struct MeshResource_Provider_TYP MeshResource_Provider;
typedef struct Window_TYP Window;
typedef struct CAMERABUFFER_TYP CameraBuffer;
typedef struct RenderedObjectBuffers_TYP RenderedObjectBuffers;
typedef struct SwapChain_TYP SwapChain;
typedef struct GizmoBuffer_TYP GizmoBuffer;
typedef struct RendererPipeline_Memory_TYP RendererPipeline_Memory;
typedef struct RenderV2_TYP RenderV2;


typedef struct GlobalBuffersInterface_TYP
{
	CameraBuffer* CameraBuffer;
	RenderedObjectBuffers* RenderedObjectsBuffer;
}GlobalBuffersInterface, * GlobalBuffersInterface_PTR;

typedef struct RenderResourceProviderInterface_TYP
{
	MeshResource_Provider* MeshResourceProvider;
}RenderResourceProviderInterface, * RenderResourceProviderInterface_PTR;

typedef struct RenderV2Interface_TYP
{
	Window* AppWindow;
	SwapChain* SwapChain;

	RenderResourceProviderInterface Resources;

	GlobalBuffersInterface GlobalBuffer;
	RendererPipeline_Memory* WireframeRenderMemory;
	GizmoBuffer* GizmoBuffer;
}RenderV2Interface, * RenderV2Interface_PTR;

void RenderV2Interface_build(RenderV2Interface* p_renderInterface, RenderV2* p_render);