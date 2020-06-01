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
//		p_meshRenderer->->DefaultMaterialDrawCommand.Mesh = &p_meshRenderer->Mesh;

		_Render::TextureLoadInfo l_textureLoadInfo{};
		l_textureLoadInfo.Device = p_mehsRendererInfo->MeshRendererDependencies.Device;
		l_textureLoadInfo.PreRenderDeferedCommandBufferStep = p_mehsRendererInfo->MeshRendererDependencies.PreRenderDeferedCommandBufferStep;
		_Render::Texture_load(&p_meshRenderer->Texture, "E:/GameProjects/VulkanTutorial/Assets/Textures/texture.jpg", &l_textureLoadInfo);


		_Render::DefaultMaterialV2Drawer_ExternalResources l_externalResources{};
		l_externalResources.Mesh = &p_meshRenderer->Mesh;
		l_externalResources.Texture = &p_meshRenderer->Texture;

		_Render::DefaultMaterialV2DrawerAllocInfo l_defaultMaterialV2DrawerAllocInfo{};
		l_defaultMaterialV2DrawerAllocInfo.DefaultMaterial = p_meshRenderer->MeshRendererDependencies.DefaultMaterialV2;
		l_defaultMaterialV2DrawerAllocInfo.Device = p_meshRenderer->MeshRendererDependencies.Device;
		l_defaultMaterialV2DrawerAllocInfo.ExternalResource = &l_externalResources;

		_Render::DefaultMaterialV2Instance_alloc(&p_meshRenderer->DefaultMaterialV2Instance, &l_defaultMaterialV2DrawerAllocInfo);

	};

	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject)
	{
		_Render::DefaultMaterialV2Instance_setModelMatrix(&p_meshRenderer->DefaultMaterialV2Instance, l_meshUniformObject, p_meshRenderer->MeshRendererDependencies.Device);
	};

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Render::Mesh_free(&l_meshRenderer->Mesh, l_meshRenderer->MeshRendererDependencies.Device);
		_Render::Texture_free(&l_meshRenderer->Texture, l_meshRenderer->MeshRendererDependencies.Device);

		_Render::DefaultMaterialV2Instance_free(&l_meshRenderer->DefaultMaterialV2Instance, l_meshRenderer->MeshRendererDependencies.Device);
	};
}