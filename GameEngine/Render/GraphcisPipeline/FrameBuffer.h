#pragma once

#include <vector>

#include "vulkan/vulkan.h"
#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/SwapChain/Image/ImageViews.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{
	struct FrameBufferDependencies
	{
		_GraphicsPipeline::RenderPass* RenderPass;
		_SwapChain::SwapChainInfo* SwapChainInfo;
		_SwapChainImage::ImageView* ImageView;
		_Device::Device* Device;
	};

	struct FrameBuffer
	{
		FrameBufferDependencies FrameBufferDependencies;
		VkFramebuffer FrameBuffer;
	};

	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies);
	void FrameBuffer_free(FrameBuffer* p_frameBuffer);
};