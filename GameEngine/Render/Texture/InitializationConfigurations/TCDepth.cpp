#include "TCDepth.h"

#include "VulkanObjects/Hardware/Device/Device.h"

#include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/TextureLayoutTransition.h"
#include "VulkanObjects/CommandBuffer/CommandBuffers.h"

#include "Texture/ImageViews.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	VkFormat findDepthTextureFormat(Device* p_device)
	{
		FormatSupportKey p_testedFormat{};
		p_testedFormat.ImageTiling = VK_IMAGE_TILING_OPTIMAL;
		p_testedFormat.FormatFeature = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
		p_testedFormat.Format = VK_FORMAT_D32_SFLOAT;
		if (Device_isFormatSupported(p_device, &p_testedFormat))
		{
			return p_testedFormat.Format;
		}
		else
		{
			p_testedFormat.Format = VK_FORMAT_D32_SFLOAT_S8_UINT;
			if (Device_isFormatSupported(p_device, &p_testedFormat))
			{
				return p_testedFormat.Format;
			}
			else
			{
				p_testedFormat.Format = VK_FORMAT_D24_UNORM_S8_UINT;
				if (Device_isFormatSupported(p_device, &p_testedFormat))
				{
					return p_testedFormat.Format;
				}
			}
		}

	};

	void TCDepth_BuildTextureInfo(TextureInfo* p_textureInfo, Device* p_device)
	{
		p_textureInfo->ImageType = VK_IMAGE_TYPE_2D;
		p_textureInfo->AspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		p_textureInfo->MipLevels = 1;
		p_textureInfo->ArrayLayers = 1;
		p_textureInfo->Format = findDepthTextureFormat(p_device);
		p_textureInfo->Tiling = VK_IMAGE_TILING_OPTIMAL;
		p_textureInfo->Usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		p_textureInfo->SharingMode = VK_SHARING_MODE_EXCLUSIVE;
		p_textureInfo->Samples = VK_SAMPLE_COUNT_1_BIT;
	};

	void TCDepth_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewInitializationInfo)
	{
		p_imageViewInitializationInfo->viewType = VK_IMAGE_VIEW_TYPE_2D;
	};

	void tCDepth_initializationCommandBufferOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		Texture* l_texture = (Texture*)p_commentBufferOperation->UserObject;
		TextureLayoutTransition_executeTransition(p_commandBuffer->CommandBuffer, l_texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	};

	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture)
	{
		DeferredCommandBufferOperation_alloc(p_deferredCommandBufferOperation);
		p_deferredCommandBufferOperation->UserObject = p_texture;
		p_deferredCommandBufferOperation->BuildCommandBuffer = tCDepth_initializationCommandBufferOperation_buildCommandBuffer;
	};
}