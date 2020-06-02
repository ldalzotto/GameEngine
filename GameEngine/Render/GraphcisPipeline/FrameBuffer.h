#pragma once


#include <vector>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct RenderPass;
	struct SwapChainInfo;
	struct ImageView;
	struct Device;
};

namespace _GameEngine::_Render
{
	struct FrameBufferDependencies
	{
		RenderPass* RenderPass;
		SwapChainInfo* SwapChainInfo;
		Device* Device;

		ImageView* ColorImageView;
		ImageView* DepthBufferImageView;
	};

	struct FrameBuffer
	{
		FrameBufferDependencies FrameBufferDependencies;
		VkFramebuffer FrameBuffer;
	};

	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies);
	void FrameBuffer_free(FrameBuffer* p_frameBuffer);
};