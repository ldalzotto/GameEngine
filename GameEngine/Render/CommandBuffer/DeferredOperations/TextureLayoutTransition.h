#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct CommandBuffer;
	struct Texture;
}

namespace _GameEngine::_Render
{
	void TextureLayoutTransition_executeTransition(CommandBuffer* p_commandBuffer, Texture* p_texture, VkImageLayout p_oldLayout, VkImageLayout p_newLayout);
};