#pragma once

#include "Math/Vector/Vector.h"

#include "VulkanObjects/Memory/VulkanBuffer.h"

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
		size_t MaxVerticesCount;
		size_t VerticesCount;
		VulkanBuffer Buffer;
	};

	void GizmoMesh_alloc(GizmoMesh* p_gizmoMesh, size_t p_maxVerticesNb, Device* p_device);
	void GizmoMesh_free(GizmoMesh* p_gizmoMesh, Device* p_device);

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