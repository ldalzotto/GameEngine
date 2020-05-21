#pragma once

#include <vector>

#include "vulkan/vulkan.h"

#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/Device/Device.h"

namespace _GameEngine::_Render::_SwapChain
{
	struct ImageViewsDependencies
	{
		_Device::Device* Device;
	};

	struct ImageViews
	{
		ImageViewsDependencies ImageViewsDependencies;
		std::vector<VkImageView> ImageViews;
	};

	void init(ImageViews* p_imageViews, const ImageViewsDependencies& p_imageViewsDependencies, SwapChainInfo* p_swapChainInfo,	std::vector<VkImage>* p_swapChainImages);
	void ImageViews_free(ImageViews* p_imageViews);
}