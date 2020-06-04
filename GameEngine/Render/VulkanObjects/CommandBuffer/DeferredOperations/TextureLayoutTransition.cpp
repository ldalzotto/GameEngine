#include "TextureLayoutTransition.h"

#include <stdexcept>
#include "Log/Log.h"

#include "Texture/Texture.h"
#include "VulkanObjects/CommandBuffer/CommandBuffers.h"

namespace _GameEngine::_Render
{
	void TextureLayoutTransition_executeTransition(CommandBuffer* p_commandBuffer, Texture* p_texture, VkImageLayout p_oldLayout, VkImageLayout p_newLayout)
	{
		VkImageMemoryBarrier l_barrier{};
		l_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		l_barrier.oldLayout = p_oldLayout;
		l_barrier.newLayout = p_newLayout;
		l_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		l_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		l_barrier.image = p_texture->Texture;

		if (p_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			l_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else
		{
			l_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		
		l_barrier.subresourceRange.baseMipLevel = p_texture->TextureInfo.MipLevels - 1;
		l_barrier.subresourceRange.levelCount = p_texture->TextureInfo.MipLevels;
		l_barrier.subresourceRange.baseArrayLayer = p_texture->TextureInfo.ArrayLayers - 1;
		l_barrier.subresourceRange.layerCount = p_texture->TextureInfo.ArrayLayers;

		

		VkPipelineStageFlags l_sourceStage;
		VkPipelineStageFlags l_destinationStage;

		if (p_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && p_newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			l_barrier.srcAccessMask = 0;
			l_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (p_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && p_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			l_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			l_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (p_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && p_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			l_barrier.srcAccessMask = 0;
			l_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Unsupported layout transition!"));
		}

		vkCmdPipelineBarrier(p_commandBuffer->CommandBuffer, l_sourceStage, l_destinationStage, 0, 0, nullptr, 0, nullptr, 1, &l_barrier);
	};
};