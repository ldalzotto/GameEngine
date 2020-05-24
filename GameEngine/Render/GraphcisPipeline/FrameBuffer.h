#pragma once


#include <vector>

#include "vulkan/vulkan.h"
#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/SwapChain/Image/ImageViews.h"

namespace _GameEngine::_Render
{
	struct FrameBufferDependencies
	{
		RenderPass* RenderPass;
		SwapChainInfo* SwapChainInfo;
		ImageView* ImageView;
		Device* Device;
	};

	struct FrameBuffer
	{
		FrameBufferDependencies FrameBufferDependencies;
		VkFramebuffer FrameBuffer;
	};

	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies);
	void FrameBuffer_free(FrameBuffer* p_frameBuffer);
};