#include "RenderV2.h"

#include "Heap/RenderHeap.h"
#include "Renderer/Solid/SolidRenderer.h"
#include "Depth/DepthBuffer.h"
#include "Objects/SwapChain/SwapChain.h"
#include "Objects/Texture/Texture.h"
#include "Renderer/Pipeline/RendererPipelineMemory.h"
#include "Objects/RenderedObject.h"
#include "Renderer/OpenCL/OpenCLContext.h"

void GlobalBuffers_alloc(GlobalBuffers* p_buffer)
{
	Arr_Alloc_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.NotShaded_NotTextured.RenderedObjects, 0);
	Arr_Alloc_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.NotShaded_Textured.RenderedObjects, 0);
	Arr_Alloc_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.FlatShaded_NotTextured.RenderedObjects, 0);
	Arr_Alloc_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.FlatShaded_Textured.RenderedObjects, 0);
};

void GlobalBuffers_free(GlobalBuffers* p_buffer)
{
	// Arr_Free_RenderedObjectHandle(&p_buffer->RenderedObjectsBuffer.RenderedObjects);
	Arr_Free_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.NotShaded_NotTextured.RenderedObjects);
	Arr_Free_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.NotShaded_Textured.RenderedObjects);
	Arr_Free_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.FlatShaded_NotTextured.RenderedObjects);
	Arr_Free_RenderedObjectHandle(&p_buffer->RenderedObjectBuffers.FlatShaded_Textured.RenderedObjects);
};

void RenderV2_initialize(RenderV2* p_render)
{
#if HAS_OPENCL
	 OpenCLContext_Initialize(&p_render->OpenCLContext);
#endif

	RenderHeap_Alloc(&RRenderHeap);

	MeshResourceProvider_Alloc(&p_render->Resources.MeshResourceProvider);
	TextureResourceProvider_Alloc(&p_render->Resources.TextureResourceProvider);


	Window_init(&p_render->AppWindow);
	RendererPipeline_Memory_Alloc(&p_render->WireframeRenderMemory);
	GlobalBuffers_alloc(&p_render->GlobalBuffer);
	GizmoBuffer_alloc(&p_render->GizmoBuffer);

	SwapChain_Alloc(&p_render->SwapChain);
	RenderV2Interface_build(&p_render->RenderInterface, p_render);

	SwapChain_Resize(&p_render->SwapChain, p_render->AppWindow.WindowSize.Width, p_render->AppWindow.WindowSize.Height);
}

void RenderV2_render(RenderV2* p_render)
{
	if (Window_consumeSizeChangeEvent(&p_render->AppWindow))
	{
		Texture_Free_3f(&p_render->RenderTargetTexture);
		Texture_Alloc_3f(&p_render->RenderTargetTexture, p_render->AppWindow.WindowSize.Width, p_render->AppWindow.WindowSize.Height);
		DepthBuffer_Realloc(&p_render->DepthBuffer, p_render->AppWindow.WindowSize.Width, p_render->AppWindow.WindowSize.Height);
		SwapChain_Resize(&p_render->SwapChain, p_render->AppWindow.WindowSize.Width, p_render->AppWindow.WindowSize.Height);
	}

	{
		// Color3c l_color = { 0,0,0 };
		memset(p_render->RenderTargetTexture.Pixels.Memory, 0, p_render->RenderTargetTexture.Pixels.Capacity * sizeof(Vector3f));
		DepthBuffer_Clear(&p_render->DepthBuffer, &p_render->GlobalBuffer.CameraBuffer);
		/*
		Texture3c_MemoryCursor l_presentTextureCursor;
		Texture_CreateMemoryCursor_3C(&p_render->SwapChain.PresentTexture, &l_presentTextureCursor);
		while (!TextureMemCursor_IsOutofBound_3C(&l_presentTextureCursor))
		{
			*l_presentTextureCursor.Current = l_color;
			TextureMemCursor_MoveNextPixel_3C(&l_presentTextureCursor);
		}
		*/
	}

	Recti l_presentTextureClip;
	Texture_BuildClipRect_3C(&p_render->SwapChain.PresentTexture, &l_presentTextureClip);

	{
		SolidRendererInput l_wireFrameRendererInput;
		l_wireFrameRendererInput.CameraBuffer = &p_render->GlobalBuffer.CameraBuffer;
		l_wireFrameRendererInput.WindowSize = p_render->AppWindow.WindowSize;

		if(p_render->GlobalBuffer.RenderedObjectBuffers.FlatShaded_Textured.RenderedObjects.Size > 0)
		{
			RendererPipeline_Memory_Clear(&p_render->WireframeRenderMemory);
			l_wireFrameRendererInput.RenderableObjectsBuffer = &p_render->GlobalBuffer.RenderedObjectBuffers.FlatShaded_Textured;
#if HAS_OPENCL
			DrawObjects_FlatShade_Textured_GPU(&l_wireFrameRendererInput, &p_render->OpenCLContext, &p_render->RenderTargetTexture, &l_presentTextureClip, &p_render->DepthBuffer, &p_render->WireframeRenderMemory);
#else
			DrawObjects_FlatShade_Textured(&l_wireFrameRendererInput, &p_render->RenderTargetTexture, &l_presentTextureClip, &p_render->DepthBuffer, &p_render->WireframeRenderMemory);
#endif
		}
		if (p_render->GlobalBuffer.RenderedObjectBuffers.FlatShaded_NotTextured.RenderedObjects.Size > 0)
		{
			RendererPipeline_Memory_Clear(&p_render->WireframeRenderMemory);
			l_wireFrameRendererInput.RenderableObjectsBuffer = &p_render->GlobalBuffer.RenderedObjectBuffers.FlatShaded_NotTextured;
			DrawObjects_FlatShade_NotTextured(&l_wireFrameRendererInput, &p_render->RenderTargetTexture, &l_presentTextureClip, &p_render->DepthBuffer, &p_render->WireframeRenderMemory);
		}
		if (p_render->GlobalBuffer.RenderedObjectBuffers.NotShaded_NotTextured.RenderedObjects.Size > 0)
		{
			RendererPipeline_Memory_Clear(&p_render->WireframeRenderMemory);
			l_wireFrameRendererInput.RenderableObjectsBuffer = &p_render->GlobalBuffer.RenderedObjectBuffers.NotShaded_NotTextured;
			DrawObjects_NoShade_NotTextured(&l_wireFrameRendererInput, &p_render->RenderTargetTexture, &l_presentTextureClip, &p_render->DepthBuffer, &p_render->WireframeRenderMemory);
		}
	}


	{
		GizmoRendererInput l_gizmoRendererInput;
		l_gizmoRendererInput.Buffer = &p_render->GizmoBuffer;
		l_gizmoRendererInput.CameraBuffer = &p_render->GlobalBuffer.CameraBuffer;
		l_gizmoRendererInput.WindowSize = p_render->AppWindow.WindowSize;
		Gizmo_Render(&l_gizmoRendererInput, &p_render->RenderTargetTexture, &l_presentTextureClip);
	}

	SwapChain_PushTexture(&p_render->SwapChain, &p_render->RenderTargetTexture);
	Window_presentTexture(&p_render->AppWindow, &p_render->SwapChain.PresentTexture);
};

void RenderV2_free(RenderV2* p_render)
{
	GizmoBuffer_free(&p_render->GizmoBuffer);
	GlobalBuffers_free(&p_render->GlobalBuffer);
	SwapChain_Free(&p_render->SwapChain);
	DepthBuffer_Free(&p_render->DepthBuffer);
	RendererPipeline_Memory_Free(&p_render->WireframeRenderMemory);
	TextureResourceProvider_Free(&p_render->Resources.TextureResourceProvider);
	MeshResourceProvider_Free(&p_render->Resources.MeshResourceProvider);
	RenderHeap_Free(&RRenderHeap);
}