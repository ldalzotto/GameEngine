#pragma once


#include <vector>

#include "ImageViews.h"
#include "Render/CommandBuffer/CommandBuffers.h"

#include "Render/CommandBuffer/CommandPool.h"
#include "Render/SwapChain/SwapChainSharedStructures.h"

#include "Render/Hardware/Device/Device.h"


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
		SwapChainInfo* SwapChainInfo;
		Device* Device;
		CommandPool* CommandPool;
	};

	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo);
	void SwapChainImage_free(SwapChainImage* p_swapChainImage);
};