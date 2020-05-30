#include "TextureLoadDeferredOperation.h"

#include "Log/Log.h"
#include <stdexcept>

#include "Render/CommandBuffer/CommandBuffers.h"
#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{
	void TextureLoadDeferredOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation);
	void TextureLoadDeferredOperation_onCompleted(DeferredCommandBufferOperation* p_commentBufferOperation);

	void texureLayoutTransition(CommandBuffer* p_commandBuffer, Texture* p_texture, VkImageLayout p_oldLayout, VkImageLayout p_newLayout);
	void copyBufferToImage(CommandBuffer* p_commandBuffer, VulkanBuffer* p_sourceBuffer, Texture* p_texture);

	DeferredCommandBufferOperation TextureLoadDeferredOperation_build(TextureLoadDeferredOperation** p_textureLoadDeferredOperation, DeferredCommandBufferCompletionToken** p_deferredCommandBufferCompletionToken)
	{
		DeferredCommandBufferOperation l_deferredCommandBufferOperation{};
		l_deferredCommandBufferOperation.UserObject = *p_textureLoadDeferredOperation;
		l_deferredCommandBufferOperation.BuildCommandBuffer = TextureLoadDeferredOperation_buildCommandBuffer;
		l_deferredCommandBufferOperation.OnOperationExecuted = TextureLoadDeferredOperation_onCompleted;
		l_deferredCommandBufferOperation.DeferredCommandBufferCompletionToken = *p_deferredCommandBufferCompletionToken;
		return l_deferredCommandBufferOperation;
	}

	void TextureLoadDeferredOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		TextureLoadDeferredOperation* l_textureLoadDeferredOperation = (TextureLoadDeferredOperation*)p_commentBufferOperation->UserObject;

		texureLayoutTransition(p_commandBuffer, l_textureLoadDeferredOperation->Texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(p_commandBuffer, &l_textureLoadDeferredOperation->SourceBuffer, l_textureLoadDeferredOperation->Texture);
		texureLayoutTransition(p_commandBuffer, l_textureLoadDeferredOperation->Texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	};

	void TextureLoadDeferredOperation_onCompleted(DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		TextureLoadDeferredOperation* l_textureLoadDeferredOperation = (TextureLoadDeferredOperation*)p_commentBufferOperation->UserObject;
		VulkanBuffer_free(&l_textureLoadDeferredOperation->SourceBuffer, l_textureLoadDeferredOperation->Device);
		delete l_textureLoadDeferredOperation;
	};


	void texureLayoutTransition(CommandBuffer* p_commandBuffer, Texture* p_texture, VkImageLayout p_oldLayout, VkImageLayout p_newLayout)
	{
		VkImageMemoryBarrier l_barrier{};
		l_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		l_barrier.oldLayout = p_oldLayout;
		l_barrier.newLayout = p_newLayout;
		l_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		l_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		l_barrier.image = p_texture->Texture;
		l_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
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
		else
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Unsupported layout transition!"));
		}

		vkCmdPipelineBarrier(p_commandBuffer->CommandBuffer, l_sourceStage, l_destinationStage, 0, 0, nullptr, 0, nullptr, 1, &l_barrier);
	};

	void copyBufferToImage(CommandBuffer* p_commandBuffer, VulkanBuffer* p_sourceBuffer, Texture* p_texture)
	{
		VkBufferImageCopy l_bufferImageCopy{};
		l_bufferImageCopy.bufferOffset = 0;
		l_bufferImageCopy.bufferRowLength = 0;
		l_bufferImageCopy.bufferImageHeight = 0;

		l_bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		l_bufferImageCopy.imageSubresource.mipLevel = p_texture->TextureInfo.MipLevels - 1;
		l_bufferImageCopy.imageSubresource.baseArrayLayer = p_texture->TextureInfo.ArrayLayers - 1;
		l_bufferImageCopy.imageSubresource.layerCount = p_texture->TextureInfo.ArrayLayers;

		l_bufferImageCopy.imageOffset = { 0,0,0 };
		l_bufferImageCopy.imageExtent = {
			p_texture->TextureInfo.Width,
			p_texture->TextureInfo.Height,
			p_texture->TextureInfo.Depth
		};

		vkCmdCopyBufferToImage(p_commandBuffer->CommandBuffer, p_sourceBuffer->Buffer, p_texture->Texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &l_bufferImageCopy);
	};


};