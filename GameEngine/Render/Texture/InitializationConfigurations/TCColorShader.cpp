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

	void TCColorShader_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewCreateInfo) 
	{
		p_imageViewCreateInfo->viewType = VK_IMAGE_VIEW_TYPE_2D;
		p_imageViewCreateInfo->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	};
}