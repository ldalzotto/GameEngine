#include "Gizmo.h"
#include "RenderInterface.h"

#include "Materials/MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"
#include "Materials/MaterialInstanceContainer.h"

namespace _GameEngine::_Render
{
	MaterialUniqueKey Gizmo_MaterialKey =
	{
		 "E:/GameProjects/GameEngine/Assets/Shader/out/GizmoVertex.spv",
		 "E:/GameProjects/GameEngine/Assets/Shader/out/GizmoFragment.spv"
	};

	void GizmoMesh_alloc(GizmoMesh* p_gizmoMesh, size_t p_maxVerticesNb, Device* p_device)
	{
		BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		l_bufferAllocInfo.Size = sizeof(GizmoVertex) * p_maxVerticesNb;
		VulkanBuffer_alloc(&p_gizmoMesh->Buffer, &l_bufferAllocInfo, p_device);

		p_gizmoMesh->MaxVerticesCount = p_maxVerticesNb;
		p_gizmoMesh->VerticesCount = 0;
	};

	void GizmoMesh_free(GizmoMesh* p_gizmoMesh, Device* p_device)
	{
		VulkanBuffer_free(&p_gizmoMesh->Buffer, p_device);
	};


	void Gizmo_alloc(Gizmo* p_gizmo, RenderInterface* p_renderInterface)
	{
		GizmoMesh_alloc(&p_gizmo->GizmoMesh, 2000, p_renderInterface->Device);
		p_gizmo->GizmoMaterial = MaterialResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.MaterialResourceProvider, &Gizmo_MaterialKey);
		MaterialInstanceInitInfo l_materialInstanceInitInfo{};
		l_materialInstanceInitInfo.SourceMaterial = p_gizmo->GizmoMaterial;
		p_gizmo->GizmoMaterialInstance = MaterialInstance_alloc(p_renderInterface, &l_materialInstanceInitInfo);
		MaterialInstanceContainer_addMaterialInstance(p_renderInterface->MaterialInstanceContainer, l_materialInstanceInitInfo.SourceMaterial, p_gizmo->GizmoMaterialInstance);
	};

	void Gizmo_free(Gizmo* p_gizmo, RenderInterface* p_renderInterface)
	{
		GizmoMesh_free(&p_gizmo->GizmoMesh, p_renderInterface->Device);

		MaterialInstanceContainer_removeMaterialInstance(p_renderInterface->MaterialInstanceContainer, p_gizmo->GizmoMaterial, p_gizmo->GizmoMaterialInstance);
		MaterialInstance_free(&p_gizmo->GizmoMaterialInstance);		
		_Render::MaterialResourceProvider_ReleaseResource(p_renderInterface->ResourceProvidersInterface.MaterialResourceProvider, &Gizmo_MaterialKey);
	};

	void Gizmo_populateBuffer(Gizmo* p_gizmo, RenderInterface* p_renderInterface)
	{
		GizmoVertex l_vertices[6];
		l_vertices[0].Position = { 0.0f, 0.0f , 0.0f };
		l_vertices[0].Color = { 1.0f, 0.0f , 0.0f };

		l_vertices[1].Position = { 1.0f, 0.0f , 0.0f };
		l_vertices[1].Color = { 1.0f, 0.0f , 0.0f };

		l_vertices[2].Position = { 0.0f, 0.0f , 0.0f };
		l_vertices[2].Color = { 0.0f, 1.0f , 0.0f };

		l_vertices[3].Position = { 0.0f, 1.0f , 0.0f };
		l_vertices[3].Color = { 0.0f, 1.0f , 0.0f };

		l_vertices[4].Position = { 0.0f, 0.0f , 0.0f };
		l_vertices[4].Color = { 0.0f, 0.0f , 1.0f };

		l_vertices[5].Position = { 0.0f, 0.0f , 1.0f };
		l_vertices[5].Color = { 0.0f, 0.0f , 1.0f };

		p_gizmo->GizmoMesh.VerticesCount = 6;
		VulkanBuffer_pushToGPU(&p_gizmo->GizmoMesh.Buffer, p_renderInterface->Device, l_vertices, sizeof(l_vertices));
	}
}