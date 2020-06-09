#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct TextureProceduralCreateInfo;
	struct ImageViewCreateInfo;
	struct Device;
}

namespace _GameEngine::_Render
{
	void TCColorShader_BuildTextureProceduralCreateInfo(TextureProceduralCreateInfo* p_textureProceduralCreateInfo);
	void TCColorShader_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewCreateInfo);
}