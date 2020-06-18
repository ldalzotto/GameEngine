#pragma once

#include "Math/Vector/Vector.h"

#include "VulkanObjects/Memory/VulkanBuffer.h"

#include "DataStructures/ArrayT.h"

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

	struct GizmoMesh
	{
		// TODO -> moving to a custom array without automatic resize
		_Core::ArrayT<GizmoVertex> GizmoVerticesV2;
		VulkanBuffer Buffer;
	};

	struct Gizmo
	{
		GizmoMesh GizmoMesh;
		Material* GizmoMaterial;
		MaterialInstance* GizmoMaterialInstance;
	};

	void Gizmo_alloc(Gizmo* p_gizmo, RenderInterface* p_renderInterface);
	void Gizmo_free(Gizmo* p_gizmo, RenderInterface* p_renderInterface);
	void Gizmo_populateBuffer(Gizmo* p_gizmo, RenderInterface* p_renderInterface);

}