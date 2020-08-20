#pragma once

typedef struct MESHRESOURCE_PROVIDER_TYP MESHRESOURCE_PROVIDER;
typedef struct Window_TYP Window;
typedef struct CAMERABUFFER_TYP CameraBuffer;
typedef struct RENDEREDOBJECT_BUFFER_TYP RENDEREDOBJECT_BUFFER;
typedef struct SwapChain_TYP SwapChain;
typedef struct GizmoBuffer_TYP GizmoBuffer;
typedef struct WireframeRenderer_Memory_TYP WireframeRenderer_Memory;
typedef struct RenderV2_TYP RenderV2;


typedef struct GlobalBuffersInterface_TYP
{
	CameraBuffer* CameraBuffer;
	RENDEREDOBJECT_BUFFER* RenderedObjectsBuffer;
}GlobalBuffersInterface, * GlobalBuffersInterface_PTR;

typedef struct RenderResourceProviderInterface_TYP
{
	MESHRESOURCE_PROVIDER* MeshResourceProvider;
}RenderResourceProviderInterface, * RenderResourceProviderInterface_PTR;

typedef struct RenderV2Interface_TYP
{
	Window* AppWindow;
	SwapChain* SwapChain;

	RenderResourceProviderInterface Resources;

	GlobalBuffersInterface GlobalBuffer;
	WireframeRenderer_Memory* WireframeRenderMemory;
	GizmoBuffer* GizmoBuffer;
}RenderV2Interface, * RenderV2Interface_PTR;

void RenderV2Interface_build(RenderV2Interface* p_renderInterface, RenderV2* p_render);