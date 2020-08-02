#include "Gizmo.h"
#include "RenderInterface.h"

#include "v2/Box/BoxMath.h"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Transform/TransformM.hpp"

#include "Materials/MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"
#include "Materials/MaterialInstanceContainer.h"

using namespace _MathV2;

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
			
			void* l_gizmoVerticesBufferMemory;
			VulkanBuffer_map(&p_gizmoMesh->VertexBuffer, p_device, &l_gizmoVerticesBufferMemory, l_bufferAllocInfo.Size);
			p_gizmoMesh->GizmoVerticesV2 = _Core::ArrayT_fromCStyleArray((GizmoVertex*)l_gizmoVerticesBufferMemory, p_maxVerticesNb);
		}

		{
			BufferAllocInfo l_bufferAllocInfo{};
			l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			l_bufferAllocInfo.Size = sizeof(GizmoIndiceType) * p_maxVerticesNb;
			VulkanBuffer_alloc(&p_gizmoMesh->IndicesBuffer, &l_bufferAllocInfo, p_device);
			
			void* l_gizmoIndicesBufferMemory;
			VulkanBuffer_map(&p_gizmoMesh->IndicesBuffer, p_device, &l_gizmoIndicesBufferMemory, l_bufferAllocInfo.Size);
			p_gizmoMesh->GizmoIndicesV2 = _Core::ArrayT_fromCStyleArray((GizmoIndiceType*)l_gizmoIndicesBufferMemory, p_maxVerticesNb);
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
		_Core::ArrayT_clear(&p_gizmoMesh->GizmoVerticesV2);
		_Core::ArrayT_clear(&p_gizmoMesh->GizmoIndicesV2);
	}

	void Gizmo_alloc(Gizmo* p_gizmo, RenderInterface* p_renderInterface)
	{
		gizmoMesh_alloc(&p_gizmo->GizmoMesh, 2000, p_renderInterface->Device);
		Gizmo_flushDrawStack(p_gizmo);
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
		_Core::ArrayT_pushBack(&p_gizmo->GizmoMesh.GizmoIndicesV2, &p_begin);
		_Core::ArrayT_pushBack(&p_gizmo->GizmoMesh.GizmoIndicesV2, &p_end);
	}

	void Gizmo_pushVertex(Gizmo* p_gizmo, const Vector3<float>* p_position, const Vector3<float>* p_color, GizmoIndiceType* p_out_index)
	{
		GizmoVertex l_gizmoVertex{};
		l_gizmoVertex.Position = *p_position;
		if (p_color) { l_gizmoVertex.Color = *p_color; }
		else { l_gizmoVertex.Color = { 1.0f, 1.0f, 1.0f }; }

		_Core::ArrayT_pushBack(&p_gizmo->GizmoMesh.GizmoVerticesV2, &l_gizmoVertex);
		*p_out_index = static_cast<uint16_t>(p_gizmo->GizmoMesh.GizmoVerticesV2.Size) - 1;
	};

	void Gizmo_drawPointV2(Gizmo* p_gizmo, const Vector3<float>* p_point, const Vector3<float>* p_color)
	{
		Vector3<float> tmp_vec3;
		Vector3<float> l_begin;
		Vector3<float> l_end;
		Vector3<float> l_deltaAdd;

		float l_lineLenght = 0.1f;

		{
			tmp_vec3 = { l_lineLenght, 0.0f, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_begin);
			tmp_vec3 = { -1.0f * l_lineLenght, 0.0f, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_end);

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, &l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, &l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
		{
			tmp_vec3 = { 0.0f, l_lineLenght, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_begin);
			tmp_vec3 = { 0.0f, -1.0f * l_lineLenght, 0.0f };
			VectorM::add(p_point, &tmp_vec3, &l_end);

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, &l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, &l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
		{
			tmp_vec3 = { 0.0f, 0.0f, l_lineLenght };
			VectorM::add(p_point, &tmp_vec3, &l_begin);
			tmp_vec3 = { 0.0f, 0.0f, -1.0f * l_lineLenght };
			VectorM::add(p_point, &tmp_vec3, &l_end);

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, &l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, &l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
	}

	void Gizmo_drawPoint(Gizmo* p_gizmo, const Vector3<float>* p_point)
	{
		Vector3<float> l_color = { 1.0f, 1.0f, 1.0f };
		Gizmo_drawPointV2(p_gizmo, p_point, &l_color);
	};

	void Gizmo_drawPoint(Gizmo* p_gizmo, const Vector3<float>* p_point, const Vector3<float>* p_color)
	{
		Gizmo_drawPointV2(p_gizmo, p_point, p_color);
	};

	void Gizmo_drawLine(Gizmo* p_gizmo, const Vector3<float>* p_begin, const Vector3<float>* p_end)
	{
		Vector3<float> l_color = { 1.0f, 1.0f, 1.0f };
		Gizmo_drawLine(p_gizmo, p_begin, p_end, &l_color);
	};

	void Gizmo_drawLine(Gizmo* p_gizmo, const Vector3<float>* p_begin, const Vector3<float>* p_end, const Vector3<float>* p_color)
	{
		GizmoIndiceType l_beginIndex;
		Gizmo_pushVertex(p_gizmo, p_begin, p_color, &l_beginIndex);
		GizmoIndiceType l_endIndex;
		Gizmo_pushVertex(p_gizmo, p_end, p_color, &l_endIndex);
		Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
	};

	void Gizmo_drawBox(Gizmo* p_gizmo, const Box* p_box, const Matrix<4, 4, float>* p_localToWorldMatrix, bool p_withCenter, const Vector3<float>* p_color)
	{
		BoxPoints l_boxPoints; BoxPoints_mul(Box_extractPoints(p_box, &l_boxPoints), p_localToWorldMatrix, &l_boxPoints);

		GizmoIndiceType LDF_index, LDB_index, LUF_index, RDF_index, LUB_index, RUF_index, RDB_index, RUB_index;
		{
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_D_F, p_color, &LDF_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_D_B, p_color, &LDB_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_U_F, p_color, &LUF_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_D_F, p_color, &RDF_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.L_U_B, p_color, &LUB_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_U_F, p_color, &RUF_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_D_B, p_color, &RDB_index);
			Gizmo_pushVertex(p_gizmo, &l_boxPoints.R_U_B, p_color, &RUB_index);
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
			Gizmo_drawPointV2(p_gizmo, &l_boxPoints.Center, p_color);
		}
	};

	void Gizmo_drawTransform(Gizmo* p_gizmo, Transform* p_transform)
	{
		Vector3<float> tmp_vec3_0, tmp_vec3_1, tmp_vec3_2, tmp_vec3_3;
		Gizmo_drawTransform(p_gizmo, TransformM::getWorldPosition(p_transform, &tmp_vec3_0),
			TransformM::getRight(p_transform, &tmp_vec3_1), TransformM::getUp(p_transform, &tmp_vec3_2), TransformM::getForward(p_transform, &tmp_vec3_3));
	};

	void Gizmo_drawTransform(Gizmo* p_gizmo, const Vector3<float>* p_center, const Vector3<float>* p_right, const Vector3<float>* p_up, const Vector3<float>* p_forward)
	{
		Vector3<float> tmp_vec3_0, tmp_vec3_1;
		tmp_vec3_1 = { 1.0f, 0.0f, 0.0f }; Gizmo_drawLine(p_gizmo, p_center, VectorM::add(p_center, p_right, &tmp_vec3_0), &tmp_vec3_1);
		tmp_vec3_1 = { 0.0f, 1.0f, 0.0f }; Gizmo_drawLine(p_gizmo, p_center, VectorM::add(p_center, p_up, &tmp_vec3_0), &tmp_vec3_1);
		tmp_vec3_1 = { 0.0f, 0.0f, 1.0f }; Gizmo_drawLine(p_gizmo, p_center, VectorM::add(p_center, p_forward, &tmp_vec3_0), &tmp_vec3_1);
	};
}