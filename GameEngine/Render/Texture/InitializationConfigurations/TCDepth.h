#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct ImageViewCreateInfo;
	struct DeferredCommandBufferOperation;
	struct Texture;
	struct TextureInfo;
}

namespace _GameEngine::_Render
{
	void TCDepth_BuildTextureInfo(TextureInfo* p_textureInfo, Device* p_device);
	void TCDepth_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewInitializationInfo);
	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture);
}