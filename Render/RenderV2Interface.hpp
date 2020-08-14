#pragma once

namespace _Core
{
	template<typename T, typename TT>
	struct ResourceProviderT;
}

namespace _RenderV2
{
	struct RenderV2;
	struct CameraBuffer;
	struct RenderedObjectsBuffer;
	struct MeshResource;
	struct MeshResourceKey;
	struct Window;
	struct SwapChain;
	struct WireframeRenderer_Memory;
}

namespace _RenderV2
{
	struct GlobalBuffersInterface
	{
		CameraBuffer* CameraBuffer;
		RenderedObjectsBuffer* RenderedObjectsBuffer;
	};

	struct RenderResourceProviderInterface
	{
		_Core::ResourceProviderT<MeshResource, MeshResourceKey>* MeshResourceProvider;
	};

	struct RenderV2Interface
	{
		Window* AppWindow;
		SwapChain* SwapChain;

		RenderResourceProviderInterface Resources;

		GlobalBuffersInterface GlobalBuffer;
		WireframeRenderer_Memory* WireframeRenderMemory;
	};

	void RenderV2Interface_build(RenderV2Interface* p_renderInterface, RenderV2* p_render);
}