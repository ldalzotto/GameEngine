#pragma once

#include "RenderV2Interface.hpp"

#include "Objects/Window/Window.hpp"
#include "Objects/SwapChain/SwapChain.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"

#include "Objects/Resource/MeshResourceProvider.hpp"

#include "Renderer/GlobalBuffers/CameraBuffer.hpp"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.hpp"

namespace _RenderV2
{
	struct GlobalBuffers
	{
		CameraBuffer CameraBuffer;
		RenderedObjectsBuffer RenderedObjectsBuffer;
	};

	struct RenderResourceProvider
	{
		_Core::ResourceProviderT<MeshResource, MeshResourceKey> MeshResourceProvider;
	};

	struct RenderV2
	{
		RenderV2Interface RenderInterface;

		Window AppWindow;
		SwapChain SwapChain;

		RenderResourceProvider Resources;

		GlobalBuffers GlobalBuffer;
		WireframeRenderer_Memory WireframeRenderMemory;
	};

	void RenderV2_initialize(RenderV2* p_render);
	void RenderV2_render(RenderV2* p_render);
	void RenderV2_free(RenderV2* p_render);
}