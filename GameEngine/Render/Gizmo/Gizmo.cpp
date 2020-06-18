#include "Gizmo.h"
#include "RenderInterface.h"

#include "Math/Box/BoxMath.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"

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

	void gizmoMesh_alloc(GizmoMesh* p_gizmoMesh, size_t p_maxVerticesNb, Device* p_device)
	{
		BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		l_bufferAllocInfo.Size = sizeof(GizmoVertex) * p_maxVerticesNb;
		VulkanBuffer_alloc(&p_gizmoMesh->Buffer, &l_bufferAllocInfo, p_device);

		p_gizmoMesh->GizmoVerticesV2.Array.Capacity = p_maxVerticesNb;
		p_gizmoMesh->GizmoVerticesV2.Array.Size = 0;
		p_gizmoMesh->GizmoVerticesV2.Array.ElementSize = sizeof(GizmoVertex);

		VulkanBuffer_map(&p_gizmoMesh->Buffer, p_device, &p_gizmoMesh->GizmoVerticesV2.Array.Memory, l_bufferAllocInfo.Size);
	};

	void gizmoMesh_free(GizmoMesh* p_gizmoMesh, Device* p_device)
	{
		VulkanBuffer_unMap(&p_gizmoMesh->Buffer, p_device);
		VulkanBuffer_free(&p_gizmoMesh->Buffer, p_device);
	};

	/*
	void gizmoMesh_populateBuffer(GizmoMesh* p_gizmoMesh, RenderInterface* p_renderInterface)
	{
		p_gizmoMesh->GizmoVerticesV2.clear();

		{
			GizmoVertex l_vertex =
			{
				{ 0.0f, 0.0f , 0.0f },
				{ 1.0f, 0.0f , 0.0f }
			};

			p_gizmoMesh->GizmoVerticesV2.push_back(&l_vertex);
		}
		{
			GizmoVertex l_vertex =
			{
				{ 1.0f, 0.0f , 0.0f },
				{ 1.0f, 0.0f , 0.0f }
			};

			p_gizmoMesh->GizmoVerticesV2.push_back(&l_vertex);
		}
		{
			GizmoVertex l_vertex =
			{
				{ 0.0f, 0.0f , 0.0f },
				{ 0.0f, 1.0f , 0.0f }
			};

			p_gizmoMesh->GizmoVerticesV2.push_back(&l_vertex);
		}
		{
			GizmoVertex l_vertex =
			{
				{ 0.0f, 1.0f , 0.0f },
				{ 0.0f, 1.0f , 0.0f }
			};

			p_gizmoMesh->GizmoVerticesV2.push_back(&l_vertex);
		}
		{
			GizmoVertex l_vertex =
			{
				{ 0.0f, 0.0f , 0.0f },
				{ 0.0f, 0.0f , 1.0f }
			};

			p_gizmoMesh->GizmoVerticesV2.push_back(&l_vertex);
		}
		{
			GizmoVertex l_vertex =
			{
				{ 0.0f, 0.0f , 1.0f },
				{ 0.0f, 0.0f , 1.0f }
			};

			p_gizmoMesh->GizmoVerticesV2.push_back(&l_vertex);
		}
	}
	*/

	void gizmoMesh_clearBuffer(GizmoMesh* p_gizmoMesh)
	{
		p_gizmoMesh->GizmoVerticesV2.clear();
	}

	void Gizmo_alloc(Gizmo* p_gizmo, RenderInterface* p_renderInterface)
	{
		gizmoMesh_alloc(&p_gizmo->GizmoMesh, 2000, p_renderInterface->Device);
		p_gizmo->GizmoMaterial = MaterialResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.MaterialResourceProvider, &Gizmo_MaterialKey);
		MaterialInstanceInitInfo l_materialInstanceInitInfo{};
		l_materialInstanceInitInfo.SourceMaterial = p_gizmo->GizmoMaterial;
		p_gizmo->GizmoMaterialInstance = MaterialInstance_alloc(p_renderInterface, &l_materialInstanceInitInfo);
		MaterialInstanceContainer_addMaterialInstance(p_renderInterface->MaterialInstanceContainer, l_materialInstanceInitInfo.SourceMaterial, p_gizmo->GizmoMaterialInstance);
	};

	void Gizmo_free(Gizmo* p_gizmo, RenderInterface* p_renderInterface)
	{
		gizmoMesh_free(&p_gizmo->GizmoMesh, p_renderInterface->Device);

		MaterialInstanceContainer_removeMaterialInstance(p_renderInterface->MaterialInstanceContainer, p_gizmo->GizmoMaterial, p_gizmo->GizmoMaterialInstance);
		MaterialInstance_free(&p_gizmo->GizmoMaterialInstance);
		_Render::MaterialResourceProvider_ReleaseResource(p_renderInterface->ResourceProvidersInterface.MaterialResourceProvider, &Gizmo_MaterialKey);
	};

	void Gizmo_clear(Gizmo* p_gizmo)
	{
		gizmoMesh_clearBuffer(&p_gizmo->GizmoMesh);
	}

	void Gizmo_drawLine(Gizmo* p_gizmo, _Math::Vector3f& p_begin, _Math::Vector3f& p_end, _Math::Vector3f& p_color)
	{
		{
			GizmoVertex l_gizmoVertex{};
			l_gizmoVertex.Position = p_begin;
			l_gizmoVertex.Color = p_color;
			p_gizmo->GizmoMesh.GizmoVerticesV2.push_back(&l_gizmoVertex);
		}
		{
			GizmoVertex l_gizmoVertex{};
			l_gizmoVertex.Position = p_end;
			l_gizmoVertex.Color = p_color;
			p_gizmo->GizmoMesh.GizmoVerticesV2.push_back(&l_gizmoVertex);
		}
	}

	void Gizmo_drawBox(Gizmo* p_gizmo, _Math::Box* p_box, _Math::Matrix4x4f* p_localToWorldMatrix)
	{
		_Core::ArrayT<_Math::Vector3f> l_points;
		_Math::BoxPoints l_boxPoints;
		_Math::Box_extractPoints(p_box, &l_boxPoints);
		_Math::BoxPoints_mul(&l_boxPoints, p_localToWorldMatrix);

		_Math::Vector3f l_color = { 1.0f, 1.0f, 1.0f };
		Gizmo_drawLine(p_gizmo, l_boxPoints.L_D_F, l_boxPoints.L_D_B, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.L_D_F, l_boxPoints.L_U_F, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.L_D_F, l_boxPoints.R_D_F, l_color);

		Gizmo_drawLine(p_gizmo, l_boxPoints.L_U_F, l_boxPoints.L_U_B, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.L_U_F, l_boxPoints.R_U_F, l_color);

		Gizmo_drawLine(p_gizmo, l_boxPoints.L_D_B, l_boxPoints.L_U_B, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.L_D_B, l_boxPoints.R_D_B, l_color);
		
		Gizmo_drawLine(p_gizmo, l_boxPoints.R_D_F, l_boxPoints.R_U_F, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.R_D_F, l_boxPoints.R_D_B, l_color);

		Gizmo_drawLine(p_gizmo, l_boxPoints.R_U_B, l_boxPoints.R_D_B, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.R_U_B, l_boxPoints.L_U_B, l_color);
		Gizmo_drawLine(p_gizmo, l_boxPoints.R_U_B, l_boxPoints.R_U_F, l_color);

		
	};
}