#include "TCDepth.h"

#include "VulkanObjects/Hardware/Device/Device.h"

#include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/TextureLayoutTransition.h"

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

	void TCDepth_BuildTextureProceduralCreateInfo(TextureProceduralCreateInfo* p_textureProceduralCreateInfo, Device* p_device)
	{
		p_textureProceduralCreateInfo->imageType = VK_IMAGE_TYPE_2D;
		p_textureProceduralCreateInfo->mipLevels = 1;
		p_textureProceduralCreateInfo->arrayLayers = 1;
		p_textureProceduralCreateInfo->format = findDepthTextureFormat(p_device);
		p_textureProceduralCreateInfo->tiling = VK_IMAGE_TILING_OPTIMAL;
		p_textureProceduralCreateInfo->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		p_textureProceduralCreateInfo->usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		p_textureProceduralCreateInfo->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		p_textureProceduralCreateInfo->samples = VK_SAMPLE_COUNT_1_BIT;
		p_textureProceduralCreateInfo->flags = 0;
	};

	void TCDepth_BuildVkImageViewCreateInfo(ImageViewCreateInfo* p_imageViewInitializationInfo)
	{
		p_imageViewInitializationInfo->viewType = VK_IMAGE_VIEW_TYPE_2D;
		p_imageViewInitializationInfo->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	};

	void tCDepth_initializationCommandBufferOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		Texture* l_texture = (Texture*)p_commentBufferOperation->UserObject;
		TextureLayoutTransition_executeTransition(p_commandBuffer, l_texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	};

	void TCDepth_InitializationCommandBufferOperation_build(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture)
	{
		DeferredCommandBufferOperation_alloc(p_deferredCommandBufferOperation);
		p_deferredCommandBufferOperation->UserObject = p_texture;
		p_deferredCommandBufferOperation->BuildCommandBuffer = tCDepth_initializationCommandBufferOperation_buildCommandBuffer;
	};
}