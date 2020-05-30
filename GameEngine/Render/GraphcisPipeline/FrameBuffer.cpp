#include "FrameBuffer.h"

#include <stdexcept>
#include "Log/Log.h"

#include "Render/Hardware/Device/Device.h"
#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/Texture/ImageViews.h"

namespace _GameEngine::_Render
{
	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies)
	{
		p_frameBuffer->FrameBufferDependencies = *p_frameBufferDependencies;

		VkImageView l_attachments[] = { p_frameBuffer->FrameBufferDependencies.ImageView->ImageView };

		VkFramebufferCreateInfo l_frameBufferCreateInfo{};
		l_frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		l_frameBufferCreateInfo.renderPass = p_frameBuffer->FrameBufferDependencies.RenderPass->renderPass;
		l_frameBufferCreateInfo.attachmentCount = 1;
		l_frameBufferCreateInfo.pAttachments = l_attachments;
		l_frameBufferCreateInfo.width = p_frameBuffer->FrameBufferDependencies.SwapChainInfo->SwapExtend.width;
		l_frameBufferCreateInfo.height = p_frameBuffer->FrameBufferDependencies.SwapChainInfo->SwapExtend.height;
		l_frameBufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(p_frameBuffer->FrameBufferDependencies.Device->LogicalDevice.LogicalDevice, &l_frameBufferCreateInfo, nullptr, &p_frameBuffer->FrameBuffer)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create framebuffer!"));
		}

	};

	void FrameBuffer_free(FrameBuffer* p_frameBuffer)
	{
		vkDestroyFramebuffer(p_frameBuffer->FrameBufferDependencies.Device->LogicalDevice.LogicalDevice, p_frameBuffer->FrameBuffer, nullptr);
	};
}