#include "MaterialDrawStep.h"

#include "VulkanObjects/SwapChain/SwapChain.h"
#include "RenderInterface.h"
#include "Texture/Texture.h"
#include "Materials/MaterialInstanceContainer.h"
#include "Materials/Material.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/TextureLayoutTransition.h"

namespace _GameEngine::_Render
{
	void MaterialDrawStep_buildCommandBuffer(RenderInterface* p_renderInterface, VkCommandBuffer p_commandBuffer, size_t l_imageIndex)
	{
		MaterialInstanceContainer* l_materialInstanceConctainer = p_renderInterface->MaterialInstanceContainer;

		{
			VkClearColorValue l_colorClear = { 0.0f,0.0f,0.0f,1.0f };
			SwapChainImage* l_colorImage = &p_renderInterface->SwapChain->SwapChainImages.at(l_imageIndex);
			TextureLayoutTransition_executeTransition(p_commandBuffer, l_colorImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			VkImageSubresourceRange l_subResourceRange = SwapChainImage_buildSubResource(l_colorImage);
			vkCmdClearColorImage(p_commandBuffer, l_colorImage->SwapChainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &l_colorClear, 1, &l_subResourceRange);
			TextureLayoutTransition_executeTransition(p_commandBuffer, l_colorImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		}

		{
			VkClearDepthStencilValue l_depthClear = { 1.0f, 0.0f };
			TextureLayoutTransition_executeTransition(p_commandBuffer, *(p_renderInterface->DepthTexture), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			VkImageSubresourceRange l_subResourceRange = Texture_buildSubResource(*(p_renderInterface->DepthTexture));
			vkCmdClearDepthStencilImage(p_commandBuffer, (*(p_renderInterface->DepthTexture))->Texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &l_depthClear, 1, &l_subResourceRange);
			TextureLayoutTransition_executeTransition(p_commandBuffer, *(p_renderInterface->DepthTexture), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}

		for (auto&& l_materialEntry : l_materialInstanceConctainer->InstanciatedMaterials)
		{
			Material* l_defaultMaterial = l_materialEntry.first;

			GraphicsPipeline* l_graphicsPipeline = &l_defaultMaterial->FinalDrawObjects.GraphicsPipeline;
			std::vector<FrameBuffer>* l_frameBuffers = &l_graphicsPipeline->PipelineInternals.FrameBuffers;

			RenderPass* RenderPass = &l_graphicsPipeline->PipelineInternals.RenderPass;

			VkRenderPassBeginInfo l_renderPassBeginInfo{};
			l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			l_renderPassBeginInfo.renderPass = RenderPass->renderPass;
			l_renderPassBeginInfo.framebuffer = l_frameBuffers->at(l_imageIndex).FrameBuffer;
			l_renderPassBeginInfo.renderArea.offset = { 0,0 };
			l_renderPassBeginInfo.renderArea.extent = p_renderInterface->SwapChain->SwapChainInfo.SwapExtend;

			vkCmdBeginRenderPass(p_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_graphicsPipeline->PipelineInternals.Pipeline);

			for (MaterialInstance* l_materialInstance : l_materialEntry.second)
			{
				l_defaultMaterial->FinalDrawObjects.MaterialDrawFn(p_commandBuffer, l_materialInstance, p_renderInterface);
			}

			vkCmdEndRenderPass(p_commandBuffer);
		}
	}

}