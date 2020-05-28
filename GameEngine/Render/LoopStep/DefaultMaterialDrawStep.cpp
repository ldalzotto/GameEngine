#include "DefaultMaterialDrawStep.h"

#include "Render/Render.h"

#include "Utils/Algorithm/Algorithm.h"


namespace _GameEngine::_Render
{

	void DefaultMaterialDrawStep_buildCommandBuffer(Render* p_render, DefaultMaterialDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex)
	{
		GraphicsPipeline* l_graphicsPipeline = &p_meshDrawStep->DefaultMaterial->GraphicsPipeline;
		std::vector<FrameBuffer>* l_frameBuffers = &l_graphicsPipeline->PipelineInternals.FrameBuffers;

		RenderPass* RenderPass = &l_graphicsPipeline->PipelineInternals.RenderPass;

		VkRenderPassBeginInfo l_renderPassBeginInfo{};
		l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		l_renderPassBeginInfo.renderPass = RenderPass->renderPass;
		l_renderPassBeginInfo.framebuffer = l_frameBuffers->at(l_imageIndex).FrameBuffer;
		l_renderPassBeginInfo.renderArea.offset = { 0,0 };
		l_renderPassBeginInfo.renderArea.extent = p_render->SwapChain.SwapChainInfo.SwapExtend;

		VkClearValue l_clearValue = { 0.0f,0.0f,0.0f,1.0f };
		l_renderPassBeginInfo.clearValueCount = 1;
		l_renderPassBeginInfo.pClearValues = &l_clearValue;

		vkCmdBeginRenderPass(p_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_graphicsPipeline->PipelineInternals.Pipeline);


		for (DefaultMaterialDrawCommand* l_meshDrawCommand : p_meshDrawStep->MeshDrawCommands)
		{
			DefaultMaterialInstance* l_defaultMaterialInstance = &l_meshDrawCommand->DefaultMaterialInstance;

			VkBuffer l_vertexBuffers[] = { l_meshDrawCommand->Mesh->VertexBuffer.Buffer };
			VkDeviceSize l_offsets[] = { 0 };
			vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
			vkCmdBindIndexBuffer(p_commandBuffer, l_meshDrawCommand->Mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_defaultMaterialInstance->_DefaultMaterial->PipelineLayout, 1, 1, &l_defaultMaterialInstance->ModelProjectionDescriptorSet, 0, nullptr);
			vkCmdDrawIndexed(p_commandBuffer, l_meshDrawCommand->Mesh->Indices.size(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(p_commandBuffer);
	}

	void DefaultMaterialDrawStep_init(DefaultMaterialDrawStep* p_meshDrawStep, Render* p_render)
	{
		p_meshDrawStep->DefaultMaterial = &p_render->RenderMaterials.DefaultMaterial;
	};

	void DefaultMaterialDrawStep_clear(DefaultMaterialDrawStep* p_meshDrawStep, Render* p_render)
	{
		//DefaultMaterial_free(p_meshDrawStep->DefaultMaterial, p_render);
		p_meshDrawStep->MeshDrawCommands.clear();
	};
}