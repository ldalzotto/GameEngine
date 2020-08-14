#include "RenderV2.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"

#include "Objects/Texture/TextureM.hpp"

namespace _RenderV2
{
	void GlobalBuffers_alloc(GlobalBuffers* p_buffer)
	{
		_Core::VectorT_alloc(&p_buffer->RenderedObjectsBuffer.RenderedObjects, 0);
	};
	
	void GlobalBuffers_free(GlobalBuffers* p_buffer)
	{
		_Core::VectorT_free(&p_buffer->RenderedObjectsBuffer.RenderedObjects);
	};

	void RenderV2_initialize(RenderV2* p_render)
	{
		Window_init(&p_render->AppWindow);
		WireframeRenderer_Memory_alloc(&p_render->WireframeRenderMemory);
		GlobalBuffers_alloc(&p_render->GlobalBuffer);
	}

	void RenderV2_render(RenderV2* p_render)
	{
		if (Window_consumeSizeChangeEvent(&p_render->AppWindow))
		{
			if (p_render->PresentTexture.Pixels.Memory)
			{
				TextureM::freePixels(&p_render->PresentTexture);
			}
			p_render->PresentTexture.Width = p_render->AppWindow.WindowSize.Width;
			p_render->PresentTexture.Height = p_render->AppWindow.WindowSize.Height;
			TextureM::allocPixels(&p_render->PresentTexture);
		};

		{
			_MathV2::Vector3<char> l_color = { 255, 255, 255 };
			_MathV2::Vector3<char>* l_pixel = &p_render->PresentTexture.Pixels.Memory[0];
			TextureIterator<3, char> l_present_it = TextureM::buildIterator(&p_render->PresentTexture);
			while (TextureIteratorM::moveNext(&l_present_it))
			{
				*l_present_it.Current = l_color;
			}
		}

		{
			WireframeRendererInput l_wireFrameRendererInput;
			l_wireFrameRendererInput.CameraBuffer = &p_render->GlobalBuffer.CameraBuffer;
			l_wireFrameRendererInput.RenderableObjectsBuffer = &p_render->GlobalBuffer.RenderedObjectsBuffer;
			l_wireFrameRendererInput.GraphicsAPIToScreeMatrix = &p_render->AppWindow.GraphicsAPIToWindowPixelCoordinates;
			WireframeRenderer_renderV2(&l_wireFrameRendererInput, &p_render->PresentTexture, &TextureM::buildClipRect(&p_render->PresentTexture), &p_render->WireframeRenderMemory);
		}
		Window_presentTexture(&p_render->AppWindow, &p_render->PresentTexture);
	};

	void RenderV2_free(RenderV2* p_render)
	{
		GlobalBuffers_free(&p_render->GlobalBuffer);
		TextureM::freePixels(&p_render->PresentTexture);
		WireframeRenderer_Memory_free(&p_render->WireframeRenderMemory);
	}
}