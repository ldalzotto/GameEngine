#include "Gizmo.h"
#include "RenderInterface.h"

#include "Math/Box/BoxMath.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Transform/Transform.h"

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
		{
			BufferAllocInfo l_bufferAllocInfo{};
			l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			l_bufferAllocInfo.Size = sizeof(GizmoVertex) * p_maxVerticesNb;
			VulkanBuffer_alloc(&p_gizmoMesh->VertexBuffer, &l_bufferAllocInfo, p_device);
			p_gizmoMesh->GizmoVerticesV2.Array.Capacity = p_maxVerticesNb;
			p_gizmoMesh->GizmoVerticesV2.Array.Size = 0;
			p_gizmoMesh->GizmoVerticesV2.Array.ElementSize = sizeof(GizmoVertex);
			VulkanBuffer_map(&p_gizmoMesh->VertexBuffer, p_device, &p_gizmoMesh->GizmoVerticesV2.Array.Memory, l_bufferAllocInfo.Size);
		}

		{
			BufferAllocInfo l_bufferAllocInfo{};
			l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			l_bufferAllocInfo.Size = sizeof(GizmoIndiceType) * p_maxVerticesNb;
			VulkanBuffer_alloc(&p_gizmoMesh->IndicesBuffer, &l_bufferAllocInfo, p_device);
			p_gizmoMesh->GizmoIndicesV2.Array.Capacity = p_maxVerticesNb;
			p_gizmoMesh->GizmoIndicesV2.Array.Size = 0;
			p_gizmoMesh->GizmoIndicesV2.Array.ElementSize = sizeof(GizmoIndiceType);
			VulkanBuffer_map(&p_gizmoMesh->IndicesBuffer, p_device, &p_gizmoMesh->GizmoIndicesV2.Array.Memory, l_bufferAllocInfo.Size);
		}
	};

	void gizmoMesh_free(GizmoMesh* p_gizmoMesh, Device* p_device)
	{
		VulkanBuffer_unMap(&p_gizmoMesh->VertexBuffer, p_device);
		VulkanBuffer_free(&p_gizmoMesh->VertexBuffer, p_device);

		VulkanBuffer_unMap(&p_gizmoMesh->IndicesBuffer, p_device);
		VulkanBuffer_free(&p_gizmoMesh->IndicesBuffer, p_device);
	};

	void gizmoMesh_clearBuffer(GizmoMesh* p_gizmoMesh)
	{
		p_gizmoMesh->GizmoVerticesV2.clear();
		p_gizmoMesh->GizmoIndicesV2.clear();
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

	void Gizmo_flushDrawStack(Gizmo* p_gizmo)
	{
		gizmoMesh_clearBuffer(&p_gizmo->GizmoMesh);
	}

	void Gizmo_drawLine_indices(Gizmo* p_gizmo, GizmoIndiceType& p_begin, GizmoIndiceType& p_end)
	{
		p_gizmo->GizmoMesh.GizmoIndicesV2.push_back(&p_begin);
		p_gizmo->GizmoMesh.GizmoIndicesV2.push_back(&p_end);
	}

	void Gizmo_pushVertex(Gizmo* p_gizmo, _Math::Vector3f& p_position, _Math::Vector3f& p_color, GizmoIndiceType* p_out_index)
	{
		GizmoVertex l_gizmoVertex{};
		l_gizmoVertex.Position = p_position;
		l_gizmoVertex.Color = p_color;
		p_gizmo->GizmoMesh.GizmoVerticesV2.push_back(&l_gizmoVertex);
		*p_out_index = static_cast<uint16_t>(p_gizmo->GizmoMesh.GizmoVerticesV2.size()) - 1;
	};

	void Gizmo_drawPointV2(Gizmo* p_gizmo, _Math::Vector3f& p_point, _Math::Vector3f& p_color)
	{
		_Math::Vector3f l_begin;
		_Math::Vector3f l_end;
		_Math::Vector3f l_deltaAdd;

		float l_lineLenght = 0.1f;

		{
			l_deltaAdd = { l_lineLenght, 0.0f, 0.0f };
			_Math::Vector3f_add(&p_point, &l_deltaAdd, &l_begin);
			_Math::Vector3f_mul(&l_deltaAdd, -1.0f, &l_deltaAdd);
			_Math::Vector3f_add(&p_point, &l_deltaAdd, &l_end);

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
		{
			l_deltaAdd = { 0.0f, l_lineLenght, 0.0f };
			_Math::Vector3f_add(&p_point, &l_deltaAdd, &l_begin);
			_Math::Vector3f_mul(&l_deltaAdd, -1.0f, &l_deltaAdd);
			_Math::Vector3f_add(&p_point, &l_deltaAdd, &l_end);

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
		{
			l_deltaAdd = { 0.0f, 0.0f, l_lineLenght };
			_Math::Vector3f_add(&p_point, &l_deltaAdd, &l_begin);
			_Math::Vector3f_mul(&l_deltaAdd, -1.0f, &l_deltaAdd);
			_Math::Vector3f_add(&p_point, &l_deltaAdd, &l_end);

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
	}

	void Gizmo_drawPoint(Gizmo* p_gizmo, _Math::Vector3f* p_point)
	{
		_Math::Vector3f l_color = { 1.0f, 1.0f, 1.0f };
		Gizmo_drawPointV2(p_gizmo, *p_point, l_color);
	};

	void Gizmo_drawPoint(Gizmo* p_gizmo, _Math::Vector3f* p_point, _Math::Vector3f* p_color)
	{
		Gizmo_drawPointV2(p_gizmo, *p_point, *p_color);
	};

	void Gizmo_drawLine(Gizmo* p_gizmo, _Math::Vector3f* p_begin, _Math::Vector3f* p_end)
	{
		_Math::Vector3f l_color = { 1.0f, 1.0f, 1.0f };
		Gizmo_drawLine(p_gizmo, p_begin, p_end, &l_color);
	};

	void Gizmo_drawLine(Gizmo* p_gizmo, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Math::Vector3f* p_color)
	{
		GizmoIndiceType l_beginIndex;
		Gizmo_pushVertex(p_gizmo, *p_begin, *p_color, &l_beginIndex);
		GizmoIndiceType l_endIndex;
		Gizmo_pushVertex(p_gizmo, *p_end, *p_color, &l_endIndex);
		Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
	};

	void Gizmo_drawBox(Gizmo* p_gizmo, _Math::Box* p_box, _Math::Matrix4x4f* p_localToWorldMatrix, bool p_withCenter, _Math::Vector3f* p_color)
	{
		_Core::ArrayT<_Math::Vector3f> l_points;
		_Math::BoxPoints l_boxPoints;
		_Math::Box_extractPoints(p_box, &l_boxPoints);
		_Math::BoxPoints_mul(&l_boxPoints, p_localToWorldMatrix);

		_Math::Vector3f l_color;
		if (p_color) { l_color = *p_color; }
		else { l_color = { 1.0f, 1.0f, 1.0f }; }

		GizmoIndiceType LDF_index, LDB_index, LUF_index, RDF_index, LUB_index, RUF_index, RDB_index, RUB_index;
		{
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_D_F, l_color, &LDF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_D_B, l_color, &LDB_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_U_F, l_color, &LUF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_D_F, l_color, &RDF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_U_B, l_color, &LUB_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_U_F, l_color, &RUF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_D_B, l_color, &RDB_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_U_B, l_color, &RUB_index);
		}

		Gizmo_drawLine_indices(p_gizmo, LDF_index, LDB_index);
		Gizmo_drawLine_indices(p_gizmo, LDF_index, LUF_index);
		Gizmo_drawLine_indices(p_gizmo, LDF_index, RDF_index);
		Gizmo_drawLine_indices(p_gizmo, LUF_index, LUB_index);
		Gizmo_drawLine_indices(p_gizmo, LUF_index, RUF_index);
		Gizmo_drawLine_indices(p_gizmo, LDB_index, LUB_index);
		Gizmo_drawLine_indices(p_gizmo, LDB_index, RDB_index);
		Gizmo_drawLine_indices(p_gizmo, RDF_index, RUF_index);
		Gizmo_drawLine_indices(p_gizmo, RDF_index, RDB_index);
		Gizmo_drawLine_indices(p_gizmo, RUB_index, RDB_index);
		Gizmo_drawLine_indices(p_gizmo, RUB_index, LUB_index);
		Gizmo_drawLine_indices(p_gizmo, RUB_index, RUF_index);

		if (p_withCenter)
		{
			Gizmo_drawPointV2(p_gizmo, l_boxPoints.Center, l_color);
		}
	};

	void Gizmo_drawTransform(Gizmo* p_gizmo, _Math::Transform* p_transform)
	{
		_Math::Vector3f l_center = _Math::Transform_getWorldPosition(p_transform);
		_Math::Vector3f l_right = _Math::Transform_getRight(p_transform);
		_Math::Vector3f l_up = _Math::Transform_getUp(p_transform);
		_Math::Vector3f l_forward = _Math::Transform_getForward(p_transform);
		Gizmo_drawTransform(p_gizmo, &l_center, &l_right, &l_up, &l_forward);
	};

	void Gizmo_drawTransform(Gizmo* p_gizmo, _Math::Vector3f* p_center, _Math::Vector3f* p_right, _Math::Vector3f* p_up, _Math::Vector3f* p_forward)
	{
		_Math::Vector3f l_right = *p_right;
		_Math::Vector3f_add(p_center, &l_right, &l_right);
		_Math::Vector3f l_up = *p_up;
		_Math::Vector3f_add(p_center, &l_up, &l_up);
		_Math::Vector3f l_forward = *p_forward;
		_Math::Vector3f_add(p_center, &l_forward, &l_forward);

		_Math::Vector3f l_rightColor = { 1.0f, 0.0f, 0.0f };
		_Math::Vector3f l_upColor = { 0.0f, 1.0f, 0.0f };
		_Math::Vector3f l_forwardColor = { 0.0f, 0.0f, 1.0f };


		Gizmo_drawLine(p_gizmo, p_center, &l_right, &l_rightColor);
		Gizmo_drawLine(p_gizmo, p_center, &l_up, &l_upColor);
		Gizmo_drawLine(p_gizmo, p_center, &l_forward, &l_forwardColor);
	};
}