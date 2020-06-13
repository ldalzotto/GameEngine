#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Texture;
	struct SwapChainImage;
}

namespace _GameEngine::_Render
{
	void TextureLayoutTransition_executeTransition(VkCommandBuffer p_commandBuffer, Texture* p_texture, VkImageLayout p_oldLayout, VkImageLayout p_newLayout);
	void TextureLayoutTransition_executeTransition(VkCommandBuffer p_commandBuffer, SwapChainImage* p_swapChainImage, VkImageLayout p_oldLayout, VkImageLayout p_newLayout);
};