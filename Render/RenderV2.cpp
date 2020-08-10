#include "RenderV2.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"

#include "Objects/Texture/TextureM.hpp"

namespace _RenderV2
{
	void RenderV2_initialize(RenderV2* p_render)
	{
		Window_init(&p_render->AppWindow);
	}

	void RenderV2_render(RenderV2* p_render, WireframeRendererInput* p_wireframeRenderInput)
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
			TextureM::fill(&p_render->PresentTexture, &l_color);
		}

		WirerameRenderer_render(p_wireframeRenderInput, &p_render->PresentTexture);
		Window_presentTexture(&p_render->AppWindow, &p_render->PresentTexture);
	};

	void RenderV2_free(RenderV2* p_render)
	{
		TextureM::freePixels(&p_render->PresentTexture);
	}
}