#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct ImageViewInitializationInfo;
}

namespace _GameEngine::_Render
{
	VkImageCreateInfo TCColorShader_BuildVkImageCreateInfo(uint32_t p_width, uint32_t p_height);
	VkImageViewCreateInfo TCColorShader_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo);
}