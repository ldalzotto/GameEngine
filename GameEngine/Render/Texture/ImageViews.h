#pragma once


#include <vector>
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct ImageView
	{
		VkImageView ImageView;
	};

	struct ImageViewInitializationInfo
	{
		Device* Device;
		VkImage Texture;
		VkFormat TextureFormat;
	};

	void ImageView_init(ImageView* p_imageView, ImageViewInitializationInfo* p_imageViewInitializationInfo);
	void ImageView_free(ImageView* p_imageView, Device* p_device);
}