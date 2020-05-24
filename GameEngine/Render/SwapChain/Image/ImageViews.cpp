#include "ImageViews.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render
{

	void ImageView_init(ImageView* p_imageView, ImageViewInitializationInfo* p_imageViewInitializationInfo)
	{
		p_imageView->ImageViewsDependencies = *p_imageViewInitializationInfo->ImageViewDependencies;

		VkImageViewCreateInfo l_imageCreate{};
		l_imageCreate.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		l_imageCreate.image = p_imageViewInitializationInfo->SwapChainImage;
		l_imageCreate.viewType = VK_IMAGE_VIEW_TYPE_2D;
		l_imageCreate.format = p_imageViewInitializationInfo->SwapChainInfo->SurfaceFormat.format;
		l_imageCreate.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		l_imageCreate.subresourceRange.baseMipLevel = 0;
		l_imageCreate.subresourceRange.levelCount = 1;
		l_imageCreate.subresourceRange.baseArrayLayer = 0;
		l_imageCreate.subresourceRange.layerCount = 1;

		if (vkCreateImageView(p_imageView->ImageViewsDependencies.Device->LogicalDevice.LogicalDevice, &l_imageCreate, nullptr, &(p_imageView->ImageView))
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create image views!"));
		}
	};

	void ImageView_free(ImageView* p_imageView)
	{
		vkDestroyImageView(p_imageView->ImageViewsDependencies.Device->LogicalDevice.LogicalDevice, p_imageView->ImageView, nullptr);
	};
}