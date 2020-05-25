#include "MeshRenderer.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(Component* p_meshRenderer);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->Render = p_mehsRendererInfo->Render;
		p_mehsRendererInfo->AssociatedComponent->Component_freeCallback = MeshRenderer_free;

		_Render::MeshAllocInfo l_meshAllocInfo{ };

		std::vector<_Render::Vertex> l_vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};
		std::vector<uint16_t> l_inidces = {
			 0, 1, 2, 2, 3, 0
		};

		l_meshAllocInfo.Device = &p_mehsRendererInfo->Render->Device;
		l_meshAllocInfo.PreRenderStagging = &p_mehsRendererInfo->Render->PreRenderStagging;
		l_meshAllocInfo.Vertices = &l_vertices;
		l_meshAllocInfo.Indices = &l_inidces;
		_Render::Mesh_alloc(&p_meshRenderer->Mesh, &l_meshAllocInfo);
	};

	void MeshRenderer_free(Component* p_meshRenderer)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer->Child;
		_Render::Mesh_free(&l_meshRenderer->Mesh, &l_meshRenderer->Render->Device);
	};
}