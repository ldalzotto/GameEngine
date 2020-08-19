#include "RenderV2.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"

#include "Objects/SwapChain/SwapChainMethods.hpp"
#include "Objects/Texture/TextureM.hpp"

namespace _RenderV2
{
	void GlobalBuffers_alloc(GlobalBuffers* p_buffer)
	{
		// p_buffer->CameraBuffer = (CameraBuffer*)calloc(1, sizeof(CameraBuffer));
		_Core::VectorT_alloc(&p_buffer->RenderedObjectsBuffer.RenderedObjects, 0);
	};

	void GlobalBuffers_free(GlobalBuffers* p_buffer)
	{
		// free(p_buffer->CameraBuffer);
		_Core::VectorT_free(&p_buffer->RenderedObjectsBuffer.RenderedObjects);
	};

	void RenderV2_initialize(RenderV2* p_render)
	{
		_Core::ResourceAllocatorT<MeshResource, MeshResourceKey> l_meshResourceAllocator;
		l_meshResourceAllocator.AllocFn = (_Core::AllocateResourceT_Function<MeshResource, MeshResourceKey>) MeshResource_allocate;
		l_meshResourceAllocator.FreeFn = (_Core::FreeResourceT_Function<MeshResource>) MeshResource_free;
		_Core::ResourceProviderT_alloc(&p_render->Resources.MeshResourceProvider, &l_meshResourceAllocator);


		Window_init(&p_render->AppWindow);
		WireframeRenderer_Memory_alloc(&p_render->WireframeRenderMemory);
		GlobalBuffers_alloc(&p_render->GlobalBuffer);
		GizmoBuffer_alloc(&p_render->GizmoBuffer);

		SwapChainM::alloc(&p_render->SwapChain, &p_render->RenderInterface);
		RenderV2Interface_build(&p_render->RenderInterface, p_render);

		SwapChainM::resize(&p_render->SwapChain, p_render->AppWindow.WindowSize.Width, p_render->AppWindow.WindowSize.Height);
	}

	void RenderV2_render(RenderV2* p_render)
	{
		if (Window_consumeSizeChangeEvent(&p_render->AppWindow))
		{
			SwapChainM::resize(&p_render->SwapChain, p_render->AppWindow.WindowSize.Width, p_render->AppWindow.WindowSize.Height);
		}

		{
			VECTOR3C l_color = { 0,0,0 };
			VECTOR3C_PTR l_pixel = &p_render->SwapChain.PresentTexture.Pixels.Memory[0];
			TextureIterator3C l_present_it = TextureM::buildIterator(&p_render->SwapChain.PresentTexture);
			while (TextureIteratorM::moveNext(&l_present_it))
			{
				*l_present_it.Current = l_color;
			}
		}

		{
			WireframeRendererInput l_wireFrameRendererInput;
			l_wireFrameRendererInput.CameraBuffer = &p_render->GlobalBuffer.CameraBuffer;
			l_wireFrameRendererInput.RenderableObjectsBuffer = &p_render->GlobalBuffer.RenderedObjectsBuffer;
			l_wireFrameRendererInput.GraphicsAPIToScreeMatrix = (MATRIX4F_PTR) &p_render->AppWindow.GraphicsAPIToWindowPixelCoordinates;
			WireframeRenderer_renderV2(&l_wireFrameRendererInput, &p_render->SwapChain.PresentTexture, &TextureM::buildClipRect(&p_render->SwapChain.PresentTexture), &p_render->WireframeRenderMemory);
		}
		{
			GizmoRendererInput l_gizmoRendererInput;
			l_gizmoRendererInput.Buffer = &p_render->GizmoBuffer;
			l_gizmoRendererInput.CameraBuffer = &p_render->GlobalBuffer.CameraBuffer;
			l_gizmoRendererInput.GraphicsAPIToScreeMatrix = &p_render->AppWindow.GraphicsAPIToWindowPixelCoordinates;
			Gizmo::render(&l_gizmoRendererInput, &p_render->SwapChain.PresentTexture, &TextureM::buildClipRect(&p_render->SwapChain.PresentTexture), &p_render->WireframeRenderMemory.RasterizedPixelsBuffer);
		}
		Window_presentTexture(&p_render->AppWindow, &p_render->SwapChain.PresentTexture);
	};

	void RenderV2_free(RenderV2* p_render)
	{
		GizmoBuffer_free(&p_render->GizmoBuffer);
		GlobalBuffers_free(&p_render->GlobalBuffer);
		SwapChainM::free(&p_render->SwapChain);
		WireframeRenderer_Memory_free(&p_render->WireframeRenderMemory);
		_Core::ResourceProviderT_free(&p_render->Resources.MeshResourceProvider);
	}
}