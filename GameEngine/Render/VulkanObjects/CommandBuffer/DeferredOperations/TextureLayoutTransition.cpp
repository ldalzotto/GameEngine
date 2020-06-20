#include "TextureLayoutTransition.h"

#include <stdexcept>
#include "MYLog/MYLog.h"

#include "Texture/Texture.h"
#include "VulkanObjects/SwapChain/Image/SwapChainImage.h"

namespace _GameEngine::_Render
{
	void buildStage(VkImageLayout p_oldLayout, VkImageLayout p_newLayout, VkImageMemoryBarrier* l_barrier, VkPipelineStageFlags& l_sourceStage, VkPipelineStageFlags& l_destinationStage);

	void TextureLayoutTransition_executeTransition(VkCommandBuffer p_commandBuffer, Texture* p_texture, VkImageLayout p_oldLayout, VkImageLayout p_newLayout)
	{
		VkImageMemoryBarrier l_barrier{};
		l_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		l_barrier.oldLayout = p_oldLayout;
		l_barrier.newLayout = p_newLayout;
		l_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		l_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		l_barrier.image = p_texture->Texture;


		l_barrier.subresourceRange.aspectMask = p_texture->TextureInfo.AspectMask;
		l_barrier.subresourceRange.baseMipLevel = p_texture->TextureInfo.MipLevels - 1;
		l_barrier.subresourceRange.levelCount = p_texture->TextureInfo.MipLevels;
		l_barrier.subresourceRange.baseArrayLayer = p_texture->TextureInfo.ArrayLayers - 1;
		l_barrier.subresourceRange.layerCount = p_texture->TextureInfo.ArrayLayers;

		

		VkPipelineStageFlags l_sourceStage;
		VkPipelineStageFlags l_destinationStage;

		buildStage(p_oldLayout, p_newLayout, &l_barrier, l_sourceStage, l_destinationStage);

		vkCmdPipelineBarrier(p_commandBuffer, l_sourceStage, l_destinationStage, 0, 0, nullptr, 0, nullptr, 1, &l_barrier);
	}

	void TextureLayoutTransition_executeTransition(VkCommandBuffer p_commandBuffer, SwapChainImage* p_swapChainImage, VkImageLayout p_oldLayout, VkImageLayout p_newLayout)
	{
		VkImageMemoryBarrier p_imageMemoryBarrier{};
		p_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		p_imageMemoryBarrier.oldLayout = p_oldLayout;
		p_imageMemoryBarrier.newLayout = p_newLayout;
		p_imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		p_imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		p_imageMemoryBarrier.image = p_swapChainImage->SwapChainImage;

		if (p_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			p_imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else
		{
			p_imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}


		p_imageMemoryBarrier.subresourceRange.baseMipLevel = p_swapChainImage->ImageViewCreateInfo.MipLevels - 1;
		p_imageMemoryBarrier.subresourceRange.levelCount = p_swapChainImage->ImageViewCreateInfo.MipLevels;
		p_imageMemoryBarrier.subresourceRange.baseArrayLayer = p_swapChainImage->ImageViewCreateInfo.ArrayLayers - 1;
		p_imageMemoryBarrier.subresourceRange.layerCount = p_swapChainImage->ImageViewCreateInfo.ArrayLayers;

		VkPipelineStageFlags l_sourceStage;
		VkPipelineStageFlags l_destinationStage;

		buildStage(p_oldLayout, p_newLayout, &p_imageMemoryBarrier, l_sourceStage, l_destinationStage);

		vkCmdPipelineBarrier(p_commandBuffer, l_sourceStage, l_destinationStage, 0, 0, nullptr, 0, nullptr, 1, &p_imageMemoryBarrier);
	}

	void buildStage(VkImageLayout p_oldLayout, VkImageLayout p_newLayout, VkImageMemoryBarrier* l_barrier, VkPipelineStageFlags& l_sourceStage, VkPipelineStageFlags& l_destinationStage)
	{
		if (p_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && p_newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			l_barrier->srcAccessMask = 0;
			l_barrier->dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (p_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && p_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			l_barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			l_barrier->dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (p_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && p_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			l_barrier->srcAccessMask = 0;
			l_barrier->dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if (p_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && p_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			l_barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			l_barrier->dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if (p_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && p_newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			l_barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			l_barrier->dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

			l_sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			l_destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}
		else
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Unsupported layout transition!"));
		}
	};

};