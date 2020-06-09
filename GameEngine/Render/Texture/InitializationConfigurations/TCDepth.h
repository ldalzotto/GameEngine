#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct TextureProceduralCreateInfo;
	struct ImageViewCreateInfo;
	struct DeferredCommandBufferOperation;
	struct Texture;
}

namespace _GameEngine::_Render
{
	void TCDepth_BuildTextureProceduralCreateInfo(TextureProceduralCreateInfo* p_textureProceduralCreateInfo, Device* p_device);
	void TCDepth_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewInitializationInfo);
	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture);
}