#include "DefaultMaterialDrawStep.h"

#include "Render/Mesh/Mesh.h"
#include "Render/SwapChain/SwapChain.h"

#include "Render/Materials/DefaultMaterialV2.h"
#include "Render/Materials/DefaultMaterialV2Instance.h"

#include "Utils/Algorithm/Algorithm.h"


namespace _GameEngine::_Render
{
	void DefaultMaterialDrawStep_buildCommandBuffer(SwapChain* p_swapChain, DefaultMaterialDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex)
	{
		GraphicsPipeline* l_graphicsPipeline = &p_meshDrawStep->DefaultMaterial->FinalDrawObjects.GraphicsPipeline;
		std::vector<FrameBuffer>* l_frameBuffers = &l_graphicsPipeline->PipelineInternals.FrameBuffers;

		RenderPass* RenderPass = &l_graphicsPipeline->PipelineInternals.RenderPass;

		VkRenderPassBeginInfo l_renderPassBeginInfo{};
		l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		l_renderPassBeginInfo.renderPass = RenderPass->renderPass;
		l_renderPassBeginInfo.framebuffer = l_frameBuffers->at(l_imageIndex).FrameBuffer;
		l_renderPassBeginInfo.renderArea.offset = { 0,0 };
		l_renderPassBeginInfo.renderArea.extent = p_swapChain->SwapChainInfo.SwapExtend;

		int l_clearValuesNumber = 1;
		if (p_meshDrawStep->DefaultMaterial->InternalResources.DepthBufferTexture)
		{
			l_clearValuesNumber += 1;
		}
		std::vector<VkClearValue> l_clearValues(l_clearValuesNumber);
		l_clearValues.at(0) = { 0.0f,0.0f,0.0f,1.0f };
		
		if (p_meshDrawStep->DefaultMaterial->InternalResources.DepthBufferTexture)
		{
			l_clearValues.at(1) = { 1.0f, 0.0f };
		}

		l_renderPassBeginInfo.clearValueCount = l_clearValues.size();
		l_renderPassBeginInfo.pClearValues = l_clearValues.data();

		vkCmdBeginRenderPass(p_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_graphicsPipeline->PipelineInternals.Pipeline);


		for (DefaultMaterialV2Instance* l_materialInstance : p_meshDrawStep->DefaultMaterialV2Instance)
		{
			VkBuffer l_vertexBuffers[] = { l_materialInstance->ExternalResources.Mesh->VertexBuffer.Buffer };
			VkDeviceSize l_offsets[] = { 0 };
			vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
			vkCmdBindIndexBuffer(p_commandBuffer, l_materialInstance->ExternalResources.Mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_materialInstance->_DefaultMaterial->FinalDrawObjects.PipelineLayout, 1, 1, &l_materialInstance->MaterialDescriptorSet, 0, nullptr);
			vkCmdDrawIndexed(p_commandBuffer, l_materialInstance->ExternalResources.Mesh->Indices.size(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(p_commandBuffer);
	}

	void DefaultMaterialDrawStep_init(DefaultMaterialDrawStep* p_meshDrawStep, DefaultMaterialV2* p_defaultMaterial)
	{
		p_meshDrawStep->DefaultMaterial = p_defaultMaterial;
	};

	void DefaultMaterialDrawStep_clear(DefaultMaterialDrawStep* p_meshDrawStep)
	{
		p_meshDrawStep->DefaultMaterialV2Instance.clear();
	};
}