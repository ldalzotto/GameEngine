#include "DefaultMaterialDrawStep.h"

#include "Mesh/Mesh.h"
#include "VulkanObjects/SwapChain/SwapChain.h"

#include "RenderInterface.h"
#include "Texture/Texture.h"
#include "Materials/MaterialInstanceContainer.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceKeys.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/TextureLayoutTransition.h"

#include "Utils/Algorithm/Algorithm.h"


namespace _GameEngine::_Render
{
	void DefaultMaterialDrawStep_buildCommandBuffer(RenderInterface* p_renderInterface, VkCommandBuffer p_commandBuffer, size_t l_imageIndex)
	{
		MaterialInstanceContainer* l_materialInstanceConctainer = p_renderInterface->MaterialInstanceContainer;

		VkClearColorValue l_colorClear = { 0.0f,0.0f,0.0f,1.0f };
		SwapChainImage* l_colorImage = &p_renderInterface->SwapChain->SwapChainImages.at(l_imageIndex);
		TextureLayoutTransition_executeTransition(p_commandBuffer, l_colorImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		VkImageSubresourceRange l_subResourceRange = SwapChainImage_buildSubResource(l_colorImage);
		vkCmdClearColorImage(p_commandBuffer, l_colorImage->SwapChainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &l_colorClear, 1, &l_subResourceRange);
		TextureLayoutTransition_executeTransition(p_commandBuffer, l_colorImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

		/*
		VkClearColorValue l_depthClear = { 1.0f, 0.0f };
		vkCmdClearColorImage(p_commandBuffer, (*(p_renderInterface->DepthTexture))->Texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &l_depthClear, 1, &l_subResourceRange);
		*/

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

			int l_clearValuesNumber = 1;
			
			if (l_defaultMaterial->InternalResources.DepthBufferTexture)
			{
				l_clearValuesNumber += 1;
			}
			std::vector<VkClearValue> l_clearValues(l_clearValuesNumber);
		//	l_clearValues.at(0) = { 0.0f,0.0f,0.0f,1.0f };

			if (l_defaultMaterial->InternalResources.DepthBufferTexture)
			{
				l_clearValues.at(1) = { 1.0f, 0.0f };
			}
			l_renderPassBeginInfo.clearValueCount = l_clearValues.size();
			l_renderPassBeginInfo.pClearValues = l_clearValues.data();
		
			vkCmdBeginRenderPass(p_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_graphicsPipeline->PipelineInternals.Pipeline);

			for (MaterialInstance* l_materialInstance : l_materialEntry.second)
			{
				Mesh* l_mesh = MaterialInstance_getMesh(l_materialInstance, MATERIALINSTANCE_MESH_KEY);
				VkBuffer l_vertexBuffers[] = { l_mesh->VertexBuffer.Buffer };
				VkDeviceSize l_offsets[] = { 0 };
				vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
				vkCmdBindIndexBuffer(p_commandBuffer, l_mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_materialInstance->SourceMaterial->FinalDrawObjects.PipelineLayout, 1, 1, &l_materialInstance->MaterialDescriptorSet, 0, nullptr);
				vkCmdDrawIndexed(p_commandBuffer, l_mesh->Indices.size(), 1, 0, 0, 0);
			}

			vkCmdEndRenderPass(p_commandBuffer);
		}
	}

}