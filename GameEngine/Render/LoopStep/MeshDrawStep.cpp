#include "MeshDrawStep.h"

namespace _GameEngine::_Render
{
	void MeshDrawStep_buildCommandBuffer(MeshDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, GraphicsPipeline* p_bindedGraphicsPipeline)
	{
		for (MeshDrawCommand* l_meshDraw : p_meshDrawStep->MeshDrawCommands)
		{
			VkBuffer l_vertexBuffers[] = { l_meshDraw->Mesh->VertexBuffer.Buffer };
			VkDeviceSize l_offsets[] = { 0 };
			vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
			vkCmdBindIndexBuffer(p_commandBuffer, l_meshDraw->Mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_bindedGraphicsPipeline->PipelineLayout, 0, 1, &l_meshDraw->DescriptorSet, 0, nullptr);
			vkCmdDrawIndexed(p_commandBuffer, l_meshDraw->Mesh->Indices.size(), 1, 0, 0, 0);
		}
	}
}