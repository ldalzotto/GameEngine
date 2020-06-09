#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct TextureProceduralCreateInfo;
	struct ImageViewInitializationInfo;
	struct Device;
}

namespace _GameEngine::_Render
{
	void TCColorShader_BuildTextureProceduralCreateInfo(TextureProceduralCreateInfo* p_textureProceduralCreateInfo);
	VkImageViewCreateInfo TCColorShader_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo);
}