#pragma once

#include "DeferredCommandBufferOperation.h"
#include "Render/Memory/VulkanBuffer.h"
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct Texture;
}

namespace _GameEngine::_Render
{

	struct TextureLoadDeferredOperation
	{
		VulkanBuffer SourceBuffer;
		Texture* Texture;
		Device* Device;
	};

	DeferredCommandBufferOperation TextureLoadDeferredOperation_build(TextureLoadDeferredOperation** p_textureLoadDeferredOperation);
};