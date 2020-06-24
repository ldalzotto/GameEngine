#pragma once

#include "Math/Vector/Vector.h"

#include "VulkanObjects/Memory/VulkanBuffer.h"

#include "DataStructures/ArrayT.h"

namespace _GameEngine::_Math
{
	struct Box;
	struct Matrix4x4f;
	struct Transform;
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

	void Gizmo_drawLine(Gizmo* p_gizmo, _Math::Vector3f* p_begin, _Math::Vector3f* p_end);
	void Gizmo_drawLine(Gizmo* p_gizmo, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Math::Vector3f* p_color);
	void Gizmo_drawPoint(Gizmo* p_gizmo, _Math::Vector3f* p_point);
	void Gizmo_drawBox(Gizmo* p_gizmo, _Math::Box* p_box, _Math::Matrix4x4f* p_localToWorldMatrix, bool p_withCenter = true);
	void Gizmo_drawTransform(Gizmo* p_gizmo, _Math::Transform* p_transform);
	void Gizmo_drawTransform(Gizmo* p_gizmo, _Math::Vector3f* p_center, _Math::Vector3f* p_right, _Math::Vector3f* p_up, _Math::Vector3f* p_forward);
}