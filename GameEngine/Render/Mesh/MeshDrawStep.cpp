#include "MeshDrawStep.h"

namespace _GameEngine::_Render
{
	void MeshDrawStep_buildCommandBuffer(MeshDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer)
	{
		for (Mesh* l_mesh : p_meshDrawStep->MeshedToDraw)
		{
			VkBuffer l_vertexBuffers[] = { l_mesh->VertexBuffer.Buffer };
			VkDeviceSize l_offsets[] = { 0 };
			vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
			vkCmdBindIndexBuffer(p_commandBuffer, l_mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);

			vkCmdDrawIndexed(p_commandBuffer, l_mesh->Indices.size(), 1, 0, 0, 0);
		}
	}
}