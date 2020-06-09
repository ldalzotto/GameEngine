#include "TCColorShader.h"

#include "Texture/ImageViews.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	void TCColorShader_BuildTextureProceduralCreateInfo(TextureProceduralCreateInfo* p_textureProceduralCreateInfo)
	{
		p_textureProceduralCreateInfo->imageType = VK_IMAGE_TYPE_2D;
		p_textureProceduralCreateInfo->mipLevels = 1;
		p_textureProceduralCreateInfo->arrayLayers = 1;
		p_textureProceduralCreateInfo->format = VK_FORMAT_R8G8B8A8_SRGB;
		p_textureProceduralCreateInfo->tiling = VK_IMAGE_TILING_OPTIMAL;
		p_textureProceduralCreateInfo->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		p_textureProceduralCreateInfo->usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		p_textureProceduralCreateInfo->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		p_textureProceduralCreateInfo->samples = VK_SAMPLE_COUNT_1_BIT;
		p_textureProceduralCreateInfo->flags = 0;
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