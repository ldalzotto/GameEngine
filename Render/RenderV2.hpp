#pragma once

#include "Objects/Window/Window.hpp"
#include "Objects/Texture/Texture.hpp"
#include "Renderer/Wireframe/WireframeRenderer.hpp"

namespace _RenderV2
{
	struct WireframeRendererInput; 
}

namespace _RenderV2
{
	struct RenderV2
	{
		Window AppWindow;
		Texture<3, char> PresentTexture;
		WireframeRenderer_Memory WireframeRenderMemory;
	};

	void RenderV2_initialize(RenderV2* p_render);
	void RenderV2_render(RenderV2* p_render, WireframeRendererInput* p_wireframeRenderInput);
	void RenderV2_free(RenderV2* p_render);
}