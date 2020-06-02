#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct ImageViewInitializationInfo;
	struct Device;
}

namespace _GameEngine::_Render
{
	VkImageCreateInfo TCColorShader_BuildVkImageCreateInfo(uint32_t p_width, uint32_t p_height, Device* p_device);
	VkImageViewCreateInfo TCColorShader_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo);
}