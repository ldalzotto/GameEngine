#include "FrameBuffer.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{
	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies)
	{
		p_frameBuffer->FrameBufferDependencies = *p_frameBufferDependencies;

		_SwapChain::ImageViews* l_imageViewsDependency = p_frameBuffer->FrameBufferDependencies.ImageViews;
		RenderPass* l_renderPassDependencies = p_frameBuffer->FrameBufferDependencies.RenderPass;

		p_frameBuffer->FrameBuffers.resize(l_imageViewsDependency->ImageViews.size());


		for (size_t i = 0; i < p_frameBuffer->FrameBuffers.size(); i++)
		{
			VkImageView l_attachments[] = { l_imageViewsDependency->ImageViews[i] };

			VkFramebufferCreateInfo l_frameBufferCreateInfo{};
			l_frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			l_frameBufferCreateInfo.renderPass = p_frameBuffer->FrameBufferDependencies.RenderPass->renderPass;
			l_frameBufferCreateInfo.attachmentCount = 1;
			l_frameBufferCreateInfo.pAttachments = l_attachments;
			l_frameBufferCreateInfo.width = l_renderPassDependencies->RenderPassDependencies.SwapChain->SwapChainInfo.SwapExtend.width;
			l_frameBufferCreateInfo.height = l_renderPassDependencies->RenderPassDependencies.SwapChain->SwapChainInfo.SwapExtend.height;
			l_frameBufferCreateInfo.layers = 1;

			if (vkCreateFramebuffer(l_imageViewsDependency->ImageViewsDependencies.Device->LogicalDevice.LogicalDevice, &l_frameBufferCreateInfo, nullptr, &p_frameBuffer->FrameBuffers[i])
				!= VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create framebuffer!"));
			}
		}

	};

	void FrameBuffer_free(FrameBuffer* p_frameBuffer)
	{
		for (size_t i = 0; i < p_frameBuffer->FrameBuffers.size(); i++)
		{
			vkDestroyFramebuffer(p_frameBuffer->FrameBufferDependencies.ImageViews->ImageViewsDependencies.Device->LogicalDevice.LogicalDevice, p_frameBuffer->FrameBuffers[i], nullptr);
		}
	};
}