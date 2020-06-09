#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct TextureProceduralCreateInfo;
	struct ImageViewInitializationInfo;
	struct DeferredCommandBufferOperation;
	struct Texture;
}

namespace _GameEngine::_Render
{
	void TCDepth_BuildTextureProceduralCreateInfo(TextureProceduralCreateInfo* p_textureProceduralCreateInfo, Device* p_device);
	VkImageViewCreateInfo TCDepth_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo);
	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture);
}