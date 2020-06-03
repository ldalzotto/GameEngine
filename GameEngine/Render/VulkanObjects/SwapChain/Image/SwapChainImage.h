#pragma once

#include <vector>
#include "Render/VulkanObjects/CommandBuffer/CommandBuffers.h"
#include "Render/Texture/ImageViews.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct CommandPool;
	struct TextureInfo;
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
		TextureInfo* TextureInfo;
		Device* Device;
		CommandPool* CommandPool;
	};

	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo);
	void SwapChainImage_free(SwapChainImage* p_swapChainImage, Device* p_device);
};