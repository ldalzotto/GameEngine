#include "MeshRenderer.h"

#include "Render/Texture/TextureSamplers.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(void* p_meshRenderer, void* p_null);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->MeshRendererDependencies = p_mehsRendererInfo->MeshRendererDependencies;

		p_meshRenderer->OnComponentDetached.Closure = p_meshRenderer;
		p_meshRenderer->OnComponentDetached.Callback = MeshRenderer_free;
		_Utils::Observer_register(&p_mehsRendererInfo->AssociatedComponent->ComponentFreeEvent, &p_meshRenderer->OnComponentDetached);

		_Render::MeshAllocInfo l_meshAllocInfo{ };
		l_meshAllocInfo.Device = p_meshRenderer->MeshRendererDependencies.Device;
		l_meshAllocInfo.PreRenderDeferedCommandBufferStep = p_meshRenderer->MeshRendererDependencies.PreRenderDeferedCommandBufferStep;

		p_meshRenderer->Mesh.Vertices = {
		   {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};
		p_meshRenderer->Mesh.Indices = {
			 0, 1, 2, 2, 3, 0
		};

		_Render::Mesh_alloc(&p_meshRenderer->Mesh, &l_meshAllocInfo);
		p_meshRenderer->DefaultMaterialDrawCommand.Mesh = &p_meshRenderer->Mesh;

		_Render::DefaultMaterialInstanceAllocInfo l_defaultMaterialInstanceAllocInfo{};
		l_defaultMaterialInstanceAllocInfo.TexturePath = "G:/GameProjects/VulkanTutorial/Assets/Textures/texture.jpg";
		l_defaultMaterialInstanceAllocInfo.PreRenderDeferedCommandBufferStep = p_meshRenderer->MeshRendererDependencies.PreRenderDeferedCommandBufferStep;
		l_defaultMaterialInstanceAllocInfo.TextureSampler = p_mehsRendererInfo->TextureSamplers->DefaultSampler;
		_Render::DefaultMaterialInstance_alloc(&p_meshRenderer->DefaultMaterialDrawCommand.DefaultMaterialInstance, &l_defaultMaterialInstanceAllocInfo,
				p_meshRenderer->MeshRendererDependencies.DefaultMaterial, p_meshRenderer->MeshRendererDependencies.Device);
	};

	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection& l_meshUniformObject)
	{
		_Render::DefaultMaterialInsance_pushModelProjectionToGPU(&p_meshRenderer->DefaultMaterialDrawCommand.DefaultMaterialInstance, &l_meshUniformObject, p_meshRenderer->MeshRendererDependencies.Device);
	};

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Render::Mesh_free(&l_meshRenderer->Mesh, l_meshRenderer->MeshRendererDependencies.Device);
		_Render::DefaultMaterialInstance_free(&l_meshRenderer->DefaultMaterialDrawCommand.DefaultMaterialInstance, l_meshRenderer->MeshRendererDependencies.Device);
	};
}