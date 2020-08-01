#pragma once

#include "v2/Vector/Vector.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"
#include "VulkanObjects/Memory/VulkanBuffer.h"

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
}

namespace _GameEngine::_Math
{
	struct Box;
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
		_MathV2::Vector3<float> Position;
		_MathV2::Vector3<float> Color;
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
	void Gizmo_flushDrawStack(Gizmo* p_gizmo);

	void Gizmo_drawLine(Gizmo* p_gizmo, const _MathV2::Vector3<float>& p_begin, const _MathV2::Vector3<float>& p_end);
	void Gizmo_drawLine(Gizmo* p_gizmo, const _MathV2::Vector3<float>& p_begin, const _MathV2::Vector3<float>& p_end, const _MathV2::Vector3<float>& p_color);
	void Gizmo_drawPoint(Gizmo* p_gizmo, const _MathV2::Vector3<float>& p_point);
	void Gizmo_drawPoint(Gizmo* p_gizmo, const _MathV2::Vector3<float>& p_point, const _MathV2::Vector3<float>& p_color);
	void Gizmo_drawBox(Gizmo* p_gizmo, _Math::Box* p_box, const _MathV2::Matrix<4, 4, float>& p_localToWorldMatrix, bool p_withCenter = true, const _MathV2::Vector3<float>& p_color = _MathV2::Vector3<float>{1.0f, 1.0f, 1.0f});
	void Gizmo_drawTransform(Gizmo* p_gizmo, _Math::Transform* p_transform);
	void Gizmo_drawTransform(Gizmo* p_gizmo, const _MathV2::Vector3<float>& p_center, const _MathV2::Vector3<float>& p_right, const _MathV2::Vector3<float>& p_up, const _MathV2::Vector3<float>& p_forward);
}