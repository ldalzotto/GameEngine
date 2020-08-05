#pragma once


#include <vector>
#include "Includes/VulkanIncludes.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct ImageView;
	struct ImageViewInitializationInfo;

	struct ImageView
	{
		VkImageView ImageView;
	};

	struct ImageViewCreateInfo
	{
		VkImageViewType            viewType;
		VkImageAspectFlags		   aspectMask;
		VkFormat Format;
		uint32_t MipLevels;
		uint32_t ArrayLayers;
	};

	struct ImageViewInitializationInfo
	{
		Device* Device;
		VkImage Texture;
		ImageViewCreateInfo* ImageViewCreateInfo;
	};

	void ImageView_init(ImageView* p_imageView, ImageViewInitializationInfo* p_imageViewInitializationInfo);
	void ImageView_free(ImageView* p_imageView, Device* p_device);
}