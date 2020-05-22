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

	struct ImageViewInitializationInfo
	{
		ImageViewsDependencies* ImageViewDependencies;
		SwapChainInfo* SwapChainInfo;
		std::vector<VkImage>* SwapChainImages;
	};

	void ImageViews_init(ImageViews* p_imageViews, ImageViewInitializationInfo* p_imageViewInitializationInfo);
	void ImageViews_free(ImageViews* p_imageViews);
}