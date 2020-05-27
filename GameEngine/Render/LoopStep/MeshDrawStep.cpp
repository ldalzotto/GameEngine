#include "MeshDrawStep.h"

#include "Render/Render.h"

#include "Utils/Algorithm/Algorithm.h"


namespace _GameEngine::_Render
{
	void MeshDrawStep_buildCommandBuffer(Render* p_render, MeshDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex)
	{
		for (auto&& l_meshDrawCommandEntry : p_meshDrawStep->MeshDrawCommands)
		{
			GraphicsPipeline* l_graphicsPipeline = l_meshDrawCommandEntry.first;
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

			for (MeshDrawCommand* l_meshDraw : l_meshDrawCommandEntry.second)
			{
				VkBuffer l_vertexBuffers[] = { l_meshDraw->Mesh->VertexBuffer.Buffer };
				VkDeviceSize l_offsets[] = { 0 };
				vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
				vkCmdBindIndexBuffer(p_commandBuffer, l_meshDraw->Mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_meshDraw->UsedRenderPipeline->PipelineInternals.PipelineLayout, 0, 1, &p_render->CameraDrawStep.DescriptorSet, 0, nullptr);
				vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_meshDraw->UsedRenderPipeline->PipelineInternals.PipelineLayout, 1, 1, &l_meshDraw->DescriptorSet, 0, nullptr);
				vkCmdDrawIndexed(p_commandBuffer, l_meshDraw->Mesh->Indices.size(), 1, 0, 0, 0);
			}

			vkCmdEndRenderPass(p_commandBuffer);

		}
		
	}

	void MeshDrawStep_addMeshDrawCommand(MeshDrawStep* p_meshDrawStep, MeshDrawCommand* p_meshDrawCommand)
	{
		if (!p_meshDrawStep->MeshDrawCommands.contains(p_meshDrawCommand->UsedRenderPipeline))
		{
			p_meshDrawStep->MeshDrawCommands[p_meshDrawCommand->UsedRenderPipeline] = std::vector<MeshDrawCommand*>();
		}
		p_meshDrawStep->MeshDrawCommands[p_meshDrawCommand->UsedRenderPipeline].emplace_back(p_meshDrawCommand);
	};

	void MeshDrawStep_removeMeshDrawCommand(MeshDrawStep* p_meshDrawStep, MeshDrawCommand* p_meshDrawCommand)
	{
		if (_Utils::Vector_eraseElementEquals(p_meshDrawStep->MeshDrawCommands[p_meshDrawCommand->UsedRenderPipeline], p_meshDrawCommand))
		{
			if (p_meshDrawStep->MeshDrawCommands[p_meshDrawCommand->UsedRenderPipeline].size() == 0)
			{
				p_meshDrawStep->MeshDrawCommands.erase(p_meshDrawCommand->UsedRenderPipeline);
			}
		}
	};
}