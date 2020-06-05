#include "TCColorShader.h"

#include "Texture/ImageViews.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	VkImageCreateInfo TCColorShader_BuildVkImageCreateInfo(uint32_t p_width, uint32_t p_height, Device* p_device)
	{
		VkImageCreateInfo l_imageCreateInfo{};
		l_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		l_imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		l_imageCreateInfo.extent.width = p_width;
		l_imageCreateInfo.extent.height = p_height;
		l_imageCreateInfo.extent.depth = 1;
		l_imageCreateInfo.mipLevels = 1;
		l_imageCreateInfo.arrayLayers = 1;
		l_imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		l_imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		l_imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		l_imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		l_imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		l_imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		l_imageCreateInfo.flags = 0;
		return l_imageCreateInfo;
	};

	VkImageViewCreateInfo TCColorShader_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo)
	{
		VkImageViewCreateInfo l_imageCreate{};
		l_imageCreate.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		l_imageCreate.image = p_imageViewInitializationInfo->Texture;
		l_imageCreate.viewType = VK_IMAGE_VIEW_TYPE_2D;
		l_imageCreate.format = p_imageViewInitializationInfo->TextureInfo->Format;
		l_imageCreate.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		l_imageCreate.subresourceRange.baseMipLevel = p_imageViewInitializationInfo->TextureInfo->MipLevels - 1;
		l_imageCreate.subresourceRange.levelCount = p_imageViewInitializationInfo->TextureInfo->MipLevels;
		l_imageCreate.subresourceRange.baseArrayLayer = p_imageViewInitializationInfo->TextureInfo->ArrayLayers - 1;
		l_imageCreate.subresourceRange.layerCount = p_imageViewInitializationInfo->TextureInfo->ArrayLayers;
		return l_imageCreate;
	};
}