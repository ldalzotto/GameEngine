#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct ImageViewInitializationInfo;
}

namespace _GameEngine::_Render
{
	VkImageCreateInfo TCDepth_BuildVkImageCreateInfo(uint32_t p_width, uint32_t p_height);
	VkImageViewCreateInfo TCDepth_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo);
}