#include "Gizmo.h"
#include "RenderInterface.h"

#include "Math/Box/BoxMath.h"
#include "v2/Vector/VectorMath.hpp"
#include "Math/Transform/Transform.h"

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

	void Gizmo_pushVertex(Gizmo* p_gizmo, const Vector3<float>& p_position, const Vector3<float>& p_color, GizmoIndiceType* p_out_index)
	{
		GizmoVertex l_gizmoVertex{};
		l_gizmoVertex.Position = p_position;
		l_gizmoVertex.Color = p_color;

		_Core::ArrayT_pushBack(&p_gizmo->GizmoMesh.GizmoVerticesV2, &l_gizmoVertex);
		*p_out_index = static_cast<uint16_t>(p_gizmo->GizmoMesh.GizmoVerticesV2.Size) - 1;
	};

	void Gizmo_drawPointV2(Gizmo* p_gizmo, const Vector3<float>& p_point, const Vector3<float>& p_color)
	{
		Vector3<float> l_begin;
		Vector3<float> l_end;
		Vector3<float> l_deltaAdd;

		float l_lineLenght = 0.1f;

		{
			l_begin = VectorM::add(p_point, { l_lineLenght, 0.0f, 0.0f });
			l_end = VectorM::add(p_point, { -1.0f * l_lineLenght, 0.0f, 0.0f });

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
		{

			l_begin = VectorM::add(p_point, { 0.0f, l_lineLenght, 0.0f });
			l_end = VectorM::add(p_point, { 0.0f, -1.0f * l_lineLenght, 0.0f });

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
		{

			l_begin = VectorM::add(p_point, { 0.0f, 0.0f, l_lineLenght });
			l_end = VectorM::add(p_point, { 0.0f, 0.0f, -1.0f * l_lineLenght });

			GizmoIndiceType l_beginIndex;
			Gizmo_pushVertex(p_gizmo, l_begin, p_color, &l_beginIndex);

			GizmoIndiceType l_endIndex;
			Gizmo_pushVertex(p_gizmo, l_end, p_color, &l_endIndex);

			Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
		}
	}

	void Gizmo_drawPoint(Gizmo* p_gizmo, const Vector3<float>& p_point)
	{
		Gizmo_drawPointV2(p_gizmo, p_point, Vector3<float> { 1.0f, 1.0f, 1.0f });
	};

	void Gizmo_drawPoint(Gizmo* p_gizmo, const Vector3<float>& p_point, const Vector3<float>& p_color)
	{
		Gizmo_drawPointV2(p_gizmo, p_point, p_color);
	};

	void Gizmo_drawLine(Gizmo* p_gizmo, const Vector3<float>& p_begin, const Vector3<float>& p_end)
	{
		Gizmo_drawLine(p_gizmo, p_begin, p_end, Vector3<float> { 1.0f, 1.0f, 1.0f });
	};

	void Gizmo_drawLine(Gizmo* p_gizmo, const Vector3<float>& p_begin, const Vector3<float>& p_end, const Vector3<float>& p_color)
	{
		GizmoIndiceType l_beginIndex;
		Gizmo_pushVertex(p_gizmo, p_begin, p_color, &l_beginIndex);
		GizmoIndiceType l_endIndex;
		Gizmo_pushVertex(p_gizmo, p_end, p_color, &l_endIndex);
		Gizmo_drawLine_indices(p_gizmo, l_beginIndex, l_endIndex);
	};

	void Gizmo_drawBox(Gizmo* p_gizmo, _Math::Box* p_box, const Matrix<4, 4, float>& p_localToWorldMatrix, bool p_withCenter, const Vector3<float>& p_color)
	{
		_Math::BoxPoints l_boxPoints;
		_Math::Box_extractPoints(p_box, &l_boxPoints);
		_Math::BoxPoints_mul(&l_boxPoints, p_localToWorldMatrix);

		GizmoIndiceType LDF_index, LDB_index, LUF_index, RDF_index, LUB_index, RUF_index, RDB_index, RUB_index;
		{
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_D_F, p_color, &LDF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_D_B, p_color, &LDB_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_U_F, p_color, &LUF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_D_F, p_color, &RDF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.L_U_B, p_color, &LUB_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_U_F, p_color, &RUF_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_D_B, p_color, &RDB_index);
			Gizmo_pushVertex(p_gizmo, l_boxPoints.R_U_B, p_color, &RUB_index);
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
			Gizmo_drawPointV2(p_gizmo, l_boxPoints.Center, p_color);
		}
	};

	void Gizmo_drawTransform(Gizmo* p_gizmo, _Math::Transform* p_transform)
	{
		Gizmo_drawTransform(p_gizmo, _Math::Transform_getWorldPosition(p_transform), 
			_Math::Transform_getRight(p_transform), _Math::Transform_getUp(p_transform), _Math::Transform_getForward(p_transform));
	};

	void Gizmo_drawTransform(Gizmo* p_gizmo, const Vector3<float>& p_center, const Vector3<float>& p_right, const Vector3<float>& p_up, const Vector3<float>& p_forward)
	{
		Gizmo_drawLine(p_gizmo, p_center, VectorM::add(p_center, p_right), Vector3<float>{1.0f, 0.0f, 0.0f});
		Gizmo_drawLine(p_gizmo, p_center, VectorM::add(p_center, p_up), Vector3<float>{0.0f, 1.0f, 0.0f});
		Gizmo_drawLine(p_gizmo, p_center, VectorM::add(p_center, p_forward), Vector3<float>{0.0f, 0.0f, 1.0f});
	};
}