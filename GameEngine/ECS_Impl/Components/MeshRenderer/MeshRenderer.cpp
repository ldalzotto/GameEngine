#include "MeshRenderer.h"

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


		_Render::DefaultMaterialV2DrawerResourceProviderDependencies l_resourceProviderDependencies{};
		l_resourceProviderDependencies.MeshResourceProvider = p_mehsRendererInfo->MeshRendererDependencies.MeshResourceProvider;
		l_resourceProviderDependencies.TextureResourceProvider = p_mehsRendererInfo->MeshRendererDependencies.TextureResourceProvider;

		_Render::DefaultMaterialV2DrawerAllocInfo l_defaultMaterialV2DrawerAllocInfo{};
		l_defaultMaterialV2DrawerAllocInfo.DefaultMaterial = p_meshRenderer->MeshRendererDependencies.DefaultMaterialV2;
		l_defaultMaterialV2DrawerAllocInfo.Device = p_meshRenderer->MeshRendererDependencies.Device;
		l_defaultMaterialV2DrawerAllocInfo.MeshUniqueKey = { "TEST" };
		l_defaultMaterialV2DrawerAllocInfo.TextureUniqueKey = { "E:/GameProjects/VulkanTutorial/Assets/Textures/texture.jpg" };
		l_defaultMaterialV2DrawerAllocInfo.ResourceProviderDependencies = &l_resourceProviderDependencies;

		_Render::DefaultMaterialV2Instance_alloc(&p_meshRenderer->DefaultMaterialV2Instance, &l_defaultMaterialV2DrawerAllocInfo);

	};

	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject)
	{
		_Render::DefaultMaterialV2Instance_setModelMatrix(&p_meshRenderer->DefaultMaterialV2Instance, l_meshUniformObject, p_meshRenderer->MeshRendererDependencies.Device);
	};

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;

		_Render::DefaultMaterialV2DrawerResourceProviderDependencies l_resourceProviderDependencies{};
		l_resourceProviderDependencies.MeshResourceProvider = l_meshRenderer->MeshRendererDependencies.MeshResourceProvider;
		l_resourceProviderDependencies.TextureResourceProvider = l_meshRenderer->MeshRendererDependencies.TextureResourceProvider;

		_Render::DefaultMaterialV2InstanceFreeInfo l_defaultMaterialV2InstanceFreeInfo{};
		l_defaultMaterialV2InstanceFreeInfo.Device = l_meshRenderer->MeshRendererDependencies.Device;
		l_defaultMaterialV2InstanceFreeInfo.ResourceProviderDependencies = &l_resourceProviderDependencies;

		_Render::DefaultMaterialV2Instance_free(&l_meshRenderer->DefaultMaterialV2Instance, &l_defaultMaterialV2InstanceFreeInfo);
	};
}