#pragma once

#include "RenderV2Interface.hpp"

#include "Objects/Window/Window.hpp"
#include "Objects/SwapChain/SwapChain.hpp"

#include "Renderer/Wireframe/WireframeRenderer.hpp"
#include "Renderer/Gizmo/Gizmo.hpp"

extern "C"
{
#include "Objects/Resource/MeshResourceProviderV2.h"
}

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
		MESHRESOURCE_PROVIDER MeshResourceProvider;
	};

	struct RenderV2
	{
		RenderV2Interface RenderInterface;

		Window AppWindow;
		SwapChain SwapChain;

		RenderResourceProvider Resources;

		GlobalBuffers GlobalBuffer;
		WireframeRenderer_Memory WireframeRenderMemory;
		GizmoBuffer GizmoBuffer;
	};

	void RenderV2_initialize(RenderV2* p_render);
	void RenderV2_render(RenderV2* p_render);
	void RenderV2_free(RenderV2* p_render);
}