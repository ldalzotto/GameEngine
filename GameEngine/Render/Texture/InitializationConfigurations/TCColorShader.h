#pragma once

#include "Includes/VulkanIncludes.h"

namespace _GameEngine::_Render
{
	struct TextureInfo;
	struct ImageViewCreateInfo;
	struct Device;
}

namespace _GameEngine::_Render
{
	void TCColorShader_BuildTextureInfo(TextureInfo* p_textureInfo);
	void TCColorShader_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewCreateInfo);
}