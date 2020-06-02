#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct ImageViewInitializationInfo;
	struct DeferredCommandBufferOperation;
	struct Texture;
}

namespace _GameEngine::_Render
{
	VkImageCreateInfo TCDepth_BuildVkImageCreateInfo(uint32_t p_width, uint32_t p_height);
	VkImageViewCreateInfo TCDepth_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo);
	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture);
}