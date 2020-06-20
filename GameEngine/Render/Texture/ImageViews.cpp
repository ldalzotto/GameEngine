#include "ImageViews.h"

#include <stdexcept>
#include "MYLog/MYLog.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{

	void ImageView_init(ImageView* p_imageView, ImageViewInitializationInfo* p_imageViewInitializationInfo)
	{
		VkImageViewCreateInfo l_imageCreate{};
		l_imageCreate.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		l_imageCreate.image = p_imageViewInitializationInfo->Texture;
		l_imageCreate.viewType = p_imageViewInitializationInfo->ImageViewCreateInfo->viewType;
		l_imageCreate.format = p_imageViewInitializationInfo->ImageViewCreateInfo->Format;
		l_imageCreate.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.subresourceRange.aspectMask = p_imageViewInitializationInfo->ImageViewCreateInfo->aspectMask;
		l_imageCreate.subresourceRange.baseMipLevel = p_imageViewInitializationInfo->ImageViewCreateInfo->MipLevels - 1;
		l_imageCreate.subresourceRange.levelCount = p_imageViewInitializationInfo->ImageViewCreateInfo->MipLevels;
		l_imageCreate.subresourceRange.baseArrayLayer = p_imageViewInitializationInfo->ImageViewCreateInfo->ArrayLayers - 1;
		l_imageCreate.subresourceRange.layerCount = p_imageViewInitializationInfo->ImageViewCreateInfo->ArrayLayers;

		if (vkCreateImageView(p_imageViewInitializationInfo->Device->LogicalDevice.LogicalDevice, &l_imageCreate, nullptr, &(p_imageView->ImageView))
			!= VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create image views!"));
		}
	};

	void ImageView_free(ImageView* p_imageView, Device* p_device)
	{
		vkDestroyImageView(p_device->LogicalDevice.LogicalDevice, p_imageView->ImageView, nullptr);
	};
}