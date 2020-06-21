#pragma once

#include "Math/Vector/Vector.h"

#include "VulkanObjects/Memory/VulkanBuffer.h"

#include "DataStructures/ArrayT.h"

namespace _GameEngine::_Math
{
	struct Box;
	struct Matrix4x4f;
}

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct Device;
	struct Material;
	struct MaterialInstance;
}

namespace _GameEngine::_Render
{
	struct GizmoVertex
	{
		_Math::Vector3f Position;
		_Math::Vector3f Color;
	};

	typedef uint16_t GizmoIndiceType;

	struct GizmoMesh
	{
		_Core::ArrayT<GizmoVertex> GizmoVerticesV2;
		VulkanBuffer VertexBuffer;

		_Core::ArrayT<GizmoIndiceType> GizmoIndicesV2;
		VulkanBuffer IndicesBuffer;
	};

	struct Gizmo
	{
		GizmoMesh GizmoMesh;
		Material* GizmoMaterial;
		MaterialInstance* GizmoMaterialInstance;
	};

	void Gizmo_alloc(Gizmo* p_gizmo, RenderInterface* p_renderInterface);
	void Gizmo_free(Gizmo* p_gizmo, RenderInterface* p_renderInterface);
	void Gizmo_clear(Gizmo* p_gizmo);

	void Gizmo_drawBox(Gizmo* p_gizmo, _Math::Box* p_box, _Math::Matrix4x4f* p_localToWorldMatrix);

}