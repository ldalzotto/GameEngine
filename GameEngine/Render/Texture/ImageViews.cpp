#include "ImageViews.h"

#include <stdexcept>
#include "Log/Log.h"

#include "Render/VulkanObjects/Hardware/Device/Device.h"
#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{

	void ImageView_init(ImageView* p_imageView, ImageViewInitializationInfo* p_imageViewInitializationInfo)
	{
		VkImageViewCreateInfo l_imageCreate = p_imageViewInitializationInfo->ImageViewCreateInfoProvider(p_imageViewInitializationInfo);

		if (vkCreateImageView(p_imageViewInitializationInfo->Device->LogicalDevice.LogicalDevice, &l_imageCreate, nullptr, &(p_imageView->ImageView))
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create image views!"));
		}
	};

	void ImageView_free(ImageView* p_imageView, Device* p_device)
	{
		vkDestroyImageView(p_device->LogicalDevice.LogicalDevice, p_imageView->ImageView, nullptr);
	};
}