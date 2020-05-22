#pragma once

#include <vector>

#include "vulkan/vulkan.h"
#include "Render/SwapChain/ImageViews.h"
#include "Render/GraphcisPipeline/RenderPass.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{
	struct FrameBufferDependencies
	{
		_SwapChain::ImageViews* ImageViews;
		RenderPass* RenderPass;
	};

	struct FrameBuffer
	{
		FrameBufferDependencies FrameBufferDependencies;
		std::vector<VkFramebuffer> FrameBuffers;
	};

	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies);
	void FrameBuffer_free(FrameBuffer* p_frameBuffer);
};