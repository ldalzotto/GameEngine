#pragma once

#include <vector>

#include "ImageViews.h"
#include "Render/CommandBuffer/CommandBuffers.h"

#include "Render/CommandBuffer/CommandPool.h"
#include "Render/SwapChain/SwapChainSharedStructures.h"

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_SwapChainImage
{
	struct SwapChainImage
	{
		VkImage SwapChainImage;
		ImageView ImageView;
		_CommandBuffer::CommandBuffer CommandBuffer;
	};

	struct SwapChainImageInitializationInfo
	{
		VkImage CreatedImage;
		_SwapChain::SwapChainInfo* SwapChainInfo;
		_Device::Device* Device;
		_CommandBuffer::CommandPool* CommandPool;
	};

	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo);
	void SwapChainImage_free(SwapChainImage* p_swapChainImage);
};