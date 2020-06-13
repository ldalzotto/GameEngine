#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "VulkanObjects/CommandBuffer/CommandBuffers.h"
#include "Texture/ImageViews.h"
// #include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct CommandPool;
	struct RenderInterface;
//	struct DeferredCommandBufferCompletionToken;
}

namespace _GameEngine::_Render
{
	struct SwapChainImage
	{
		VkImage SwapChainImage;
		ImageView ImageView;
		ImageViewCreateInfo ImageViewCreateInfo;
		CommandBuffer CommandBuffer;

		// SmartDeferredCommandBufferCompletionToken SwapChainImageInitializationToken;
	};

	struct SwapChainImageInitializationInfo
	{
		VkImage CreatedImage;
		ImageViewCreateInfo* ImageViewCreateInfo;
		RenderInterface* RenderInterface;
	};

	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo);
	void SwapChainImage_free(SwapChainImage* p_swapChainImage, Device* p_device);
	VkImageSubresourceRange SwapChainImage_buildSubResource(SwapChainImage* p_swapChainImage);
};