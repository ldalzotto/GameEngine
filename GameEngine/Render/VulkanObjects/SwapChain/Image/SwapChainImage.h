#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "VulkanObjects/CommandBuffer/CommandBuffers.h"
#include "Texture/ImageViews.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct CommandPool;
}

namespace _GameEngine::_Render
{
	struct SwapChainImage
	{
		VkImage SwapChainImage;
		ImageView ImageView;
		CommandBuffer CommandBuffer;
	};

	struct SwapChainImageInitializationInfo
	{
		VkImage CreatedImage;
		ImageViewCreateInfo* ImageViewCreateInfo;
		Device* Device;
		CommandPool* CommandPool;
	};

	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo);
	void SwapChainImage_free(SwapChainImage* p_swapChainImage, Device* p_device);
};