#pragma once

#include "RenderV2Interface.h"


#include "Renderer/Solid/SolidRenderer.h"
#include "Renderer/Gizmo/Gizmo.h"
#include "Depth/DepthBuffer_def.h"
#include "Objects/SwapChain/SwapChain_def.h"
#include "Objects/Window/Window.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
#include "Objects/Texture/TextureResourceProvider.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"


/*
#define MATERIAL_TYPE_NotShaded_NotTextured 0
#define MATERIAL_TYPE_NotShaded_Textured 1
#define MATERIAL_TYPE_FlatShaded_Textured 2
#define MATERIAL_TYPE_FlatShaded_NotTextured 3
*/

typedef struct GlobalBuffers_TYP
{
	CAMERABUFFER CameraBuffer;
	RenderedObjectBuffers RenderedObjectBuffers;

}GlobalBuffers, *GlobalBuffers_PTR;

typedef struct RenderResourceProvider_TYP
{
	MeshResource_Provider MeshResourceProvider;
	Array_TextureResourceHandle TextureResourceProvider;
}RenderResourceProvider, * RenderResourceProvider_PTR;

typedef struct RenderV2_TYP
{
	RenderV2Interface RenderInterface;

	Window AppWindow;
	SwapChain SwapChain;

	RenderResourceProvider Resources;
	
	DepthBuffer DepthBuffer;
	RenderTexture3f RenderTargetTexture;

	GlobalBuffers GlobalBuffer;
	RendererPipeline_Memory WireframeRenderMemory;
	GizmoBuffer GizmoBuffer;
}RenderV2, * RenderV2_PTR;

void RenderV2_initialize(RenderV2* p_render);
void RenderV2_render(RenderV2* p_render);
void RenderV2_free(RenderV2* p_render);
