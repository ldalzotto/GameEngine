#include "TCColorShader.h"

#include "Texture/ImageViews.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	void TCColorShader_BuildTextureInfo(TextureInfo* p_textureInfo)
	{
		p_textureInfo->ImageType = VK_IMAGE_TYPE_2D;
		p_textureInfo->MipLevels = 1;
		p_textureInfo->ArrayLayers = 1;
		p_textureInfo->Format = VK_FORMAT_R8G8B8A8_SRGB;
		p_textureInfo->Tiling = VK_IMAGE_TILING_OPTIMAL;
		p_textureInfo->Usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		p_textureInfo->SharingMode = VK_SHARING_MODE_EXCLUSIVE;
		p_textureInfo->Samples = VK_SAMPLE_COUNT_1_BIT;
	};

	void TCColorShader_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewCreateInfo) 
	{
		p_imageViewCreateInfo->viewType = VK_IMAGE_VIEW_TYPE_2D;
		p_imageViewCreateInfo->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	};
}