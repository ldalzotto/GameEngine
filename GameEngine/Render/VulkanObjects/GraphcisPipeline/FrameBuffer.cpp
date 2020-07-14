#include "FrameBuffer.h"

#include <vector>
#include <stdexcept>

#include "Log/LogFormatting.hpp"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/SwapChain/SwapChainSharedStructures.h"
#include "VulkanObjects/GraphcisPipeline/RenderPass.h"
#include "Texture/ImageViews.h"

namespace _GameEngine::_Render
{
	void FrameBuffer_init(FrameBuffer* p_frameBuffer, FrameBufferDependencies* p_frameBufferDependencies)
	{
		p_frameBuffer->FrameBufferDependencies = *p_frameBufferDependencies;

		bool l_depthEnabled = p_frameBufferDependencies->DepthBufferImageView != nullptr;
		int l_attachmentsCount = 1;
		if (l_depthEnabled)
		{
			l_attachmentsCount += 1;
		}

		std::vector<VkImageView> l_attachments(l_attachmentsCount);

		l_attachments.at(0) = p_frameBuffer->FrameBufferDependencies.ColorImageView->ImageView;

		if (l_depthEnabled)
		{
			l_attachments.at(1) = p_frameBufferDependencies->DepthBufferImageView->ImageView;
		}

		VkFramebufferCreateInfo l_frameBufferCreateInfo{};
		l_frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		l_frameBufferCreateInfo.renderPass = p_frameBuffer->FrameBufferDependencies.RenderPass->renderPass;
		l_frameBufferCreateInfo.attachmentCount = l_attachments.size();
		l_frameBufferCreateInfo.pAttachments = l_attachments.data();
		l_frameBufferCreateInfo.width = p_frameBuffer->FrameBufferDependencies.SwapChainInfo->SwapExtend.width;
		l_frameBufferCreateInfo.height = p_frameBuffer->FrameBufferDependencies.SwapChainInfo->SwapExtend.height;
		l_frameBufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(p_frameBuffer->FrameBufferDependencies.Device->LogicalDevice.LogicalDevice, &l_frameBufferCreateInfo, nullptr, &p_frameBuffer->FrameBuffer)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create framebuffer!"));
		}

	};

	void FrameBuffer_free(FrameBuffer* p_frameBuffer)
	{
		vkDestroyFramebuffer(p_frameBuffer->FrameBufferDependencies.Device->LogicalDevice.LogicalDevice, p_frameBuffer->FrameBuffer, nullptr);
	};
}