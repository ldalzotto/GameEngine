#include "TCDepth.h"

#include "Render/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"
#include "Render/Texture/ImageViews.h"
#include "Render/Texture/Texture.h"
#include "Render/CommandBuffer/DeferredOperations/TextureLayoutTransition.h"

namespace _GameEngine::_Render
{
	VkImageCreateInfo TCDepth_BuildVkImageCreateInfo(uint32_t p_width, uint32_t p_height)
	{
		VkImageCreateInfo l_imageCreateInfo{};
		l_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		l_imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		l_imageCreateInfo.extent.width = p_width;
		l_imageCreateInfo.extent.height = p_height;
		l_imageCreateInfo.extent.depth = 1;
		l_imageCreateInfo.mipLevels = 1;
		l_imageCreateInfo.arrayLayers = 1;
		l_imageCreateInfo.format = VK_FORMAT_D32_SFLOAT;
		l_imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		l_imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		l_imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		l_imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		l_imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		l_imageCreateInfo.flags = 0;
		return l_imageCreateInfo;
	};

	VkImageViewCreateInfo TCDepth_BuildVkImageViewCreateInfo(ImageViewInitializationInfo* p_imageViewInitializationInfo)
	{
		VkImageViewCreateInfo l_imageCreate{};
		l_imageCreate.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		l_imageCreate.image = p_imageViewInitializationInfo->Texture;
		l_imageCreate.viewType = VK_IMAGE_VIEW_TYPE_2D;
		l_imageCreate.format = p_imageViewInitializationInfo->TextureInfo->Format;
		l_imageCreate.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		l_imageCreate.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		l_imageCreate.subresourceRange.baseMipLevel = p_imageViewInitializationInfo->TextureInfo->MipLevels - 1;
		l_imageCreate.subresourceRange.levelCount = p_imageViewInitializationInfo->TextureInfo->MipLevels;
		l_imageCreate.subresourceRange.baseArrayLayer = p_imageViewInitializationInfo->TextureInfo->ArrayLayers - 1;
		l_imageCreate.subresourceRange.layerCount = p_imageViewInitializationInfo->TextureInfo->ArrayLayers;
		return l_imageCreate;
	};


	void tCDepth_initializationCommandBufferOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		Texture* l_texture = (Texture*)p_commentBufferOperation->UserObject;
		// TextureLayoutTransition_executeTransition(p_commandBuffer, l_texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	};

	void tCDepth_initializationCommandBufferOperation_onCompleration(DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		Texture* l_texture = (Texture*)p_commentBufferOperation->UserObject;
		Texture_nullifyInitalizationBufferCompletionToken(l_texture);
	};

	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture)
	{
		DeferredCommandBufferOperation_alloc(p_deferredCommandBufferOperation);
		p_deferredCommandBufferOperation->UserObject = p_texture;
		p_deferredCommandBufferOperation->BuildCommandBuffer = tCDepth_initializationCommandBufferOperation_buildCommandBuffer;
		p_deferredCommandBufferOperation->OnOperationExecuted = tCDepth_initializationCommandBufferOperation_onCompleration;
	};
}