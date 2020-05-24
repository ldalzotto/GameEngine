#pragma once


#include <vector>

#include "vulkan/vulkan.h"

#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/Hardware/Device/Device.h"


namespace _GameEngine::_Render
{
	struct ImageViewsDependencies
	{
		Device* Device;
	};

	struct ImageView
	{
		ImageViewsDependencies ImageViewsDependencies;
		VkImageView ImageView;
	};

	struct ImageViewInitializationInfo
	{
		ImageViewsDependencies* ImageViewDependencies;
		SwapChainInfo* SwapChainInfo;
		VkImage SwapChainImage;
	};

	void ImageView_init(ImageView* p_imageView, ImageViewInitializationInfo* p_imageViewInitializationInfo);
	void ImageView_free(ImageView* p_imageView);
}