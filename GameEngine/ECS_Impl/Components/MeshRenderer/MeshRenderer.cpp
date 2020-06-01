#include "MeshRenderer.h"

#include "Render/Texture/TextureSamplers.h"
#include "Render/Resources/TextureResourceProvider.h"

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

		_Render::TextureUniqueKey l_textureUniqueKey{};
		l_textureUniqueKey.TexturePath = "E:/GameProjects/VulkanTutorial/Assets/Textures/texture.jpg";
		p_meshRenderer->Texture =  _Render::TextureResourceProvider_UseResource(p_mehsRendererInfo->MeshRendererDependencies.TextureResourceProvider, &l_textureUniqueKey);

		_Render::DefaultMaterialV2Drawer_ExternalResources l_externalResources{};
		l_externalResources.Mesh = &p_meshRenderer->Mesh;
		l_externalResources.Texture = p_meshRenderer->Texture;

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

		_Render::TextureResourceProvider_ReleaseResource(l_meshRenderer->MeshRendererDependencies.TextureResourceProvider, &l_meshRenderer->Texture->TextureUniqueKey);

		_Render::DefaultMaterialV2Instance_free(&l_meshRenderer->DefaultMaterialV2Instance, l_meshRenderer->MeshRendererDependencies.Device);
	};
}