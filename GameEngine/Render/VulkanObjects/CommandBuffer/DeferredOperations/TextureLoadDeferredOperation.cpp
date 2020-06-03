#include "TextureLoadDeferredOperation.h"

#include "Render/VulkanObjects/CommandBuffer/CommandBuffers.h"
#include "Render/Texture/Texture.h"

#include "Render/VulkanObjects/CommandBuffer/DeferredOperations/TextureLayoutTransition.h"

namespace _GameEngine::_Render
{
	void TextureLoadDeferredOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation);
	void TextureLoadDeferredOperation_onCompleted(DeferredCommandBufferOperation* p_commentBufferOperation);

	void copyBufferToImage(CommandBuffer* p_commandBuffer, VulkanBuffer* p_sourceBuffer, Texture* p_texture);

	DeferredCommandBufferOperation TextureLoadDeferredOperation_build(TextureLoadDeferredOperation** p_textureLoadDeferredOperation)
	{
		DeferredCommandBufferOperation l_deferredCommandBufferOperation{};
		DeferredCommandBufferOperation_alloc(&l_deferredCommandBufferOperation);
		l_deferredCommandBufferOperation.UserObject = *p_textureLoadDeferredOperation;
		l_deferredCommandBufferOperation.BuildCommandBuffer = TextureLoadDeferredOperation_buildCommandBuffer;
		l_deferredCommandBufferOperation.OnOperationExecuted = TextureLoadDeferredOperation_onCompleted;
		return l_deferredCommandBufferOperation;
	}

	void TextureLoadDeferredOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		TextureLoadDeferredOperation* l_textureLoadDeferredOperation = (TextureLoadDeferredOperation*)p_commentBufferOperation->UserObject;

		TextureLayoutTransition_executeTransition(p_commandBuffer, l_textureLoadDeferredOperation->Texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(p_commandBuffer, &l_textureLoadDeferredOperation->SourceBuffer, l_textureLoadDeferredOperation->Texture);
		TextureLayoutTransition_executeTransition(p_commandBuffer, l_textureLoadDeferredOperation->Texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	};

	void TextureLoadDeferredOperation_onCompleted(DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		TextureLoadDeferredOperation* l_textureLoadDeferredOperation = (TextureLoadDeferredOperation*)p_commentBufferOperation->UserObject;
		VulkanBuffer_free(&l_textureLoadDeferredOperation->SourceBuffer, l_textureLoadDeferredOperation->Device);
		delete l_textureLoadDeferredOperation;
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