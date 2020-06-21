#include "MaterialDrawFunctions.h"

#include "RenderInterface.h"
#include "Gizmo/Gizmo.h"
#include "Mesh/Mesh.h"
#include "Materials/MaterialInstance.h"
#include "Materials/Material.h"

namespace _GameEngine::_Render
{
	void MaterialDrawFn_meshDraw(VkCommandBuffer p_commandBuffer, MaterialInstance* l_materialInstance, RenderInterface* p_renderInterface)
	{
		Mesh* l_mesh = MaterialInstance_getMesh(l_materialInstance, MATERIALINSTANCE_MESH_KEY);
		VkBuffer l_vertexBuffers[] = { l_mesh->VertexBuffer.Buffer };
		VkDeviceSize l_offsets[] = { 0 };
		vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
		vkCmdBindIndexBuffer(p_commandBuffer, l_mesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, l_materialInstance->SourceMaterial->FinalDrawObjects.PipelineLayout, 1, 1, &l_materialInstance->MaterialDescriptorSet, 0, nullptr);
		vkCmdDrawIndexed(p_commandBuffer, l_mesh->Indices.size(), 1, 0, 0, 0);
	};
	
	void MaterialDrawFn_gizmoDraw(VkCommandBuffer p_commandBuffer, MaterialInstance*, RenderInterface* p_renderInterface)
	{
		GizmoMesh* l_gizmoMesh = &p_renderInterface->Gizmo->GizmoMesh;
		if (l_gizmoMesh->GizmoVerticesV2.size() > 0)
		{
			VkBuffer l_vertexBuffers[] = { l_gizmoMesh->VertexBuffer.Buffer };
			VkDeviceSize l_offsets[] = { 0 };
			vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
			vkCmdBindIndexBuffer(p_commandBuffer, l_gizmoMesh->IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(p_commandBuffer, l_gizmoMesh->GizmoIndicesV2.size(), 1, 0, 0, 0);
		}
	};
}