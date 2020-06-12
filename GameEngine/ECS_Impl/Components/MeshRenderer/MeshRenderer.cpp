#include "MeshRenderer.h"

#include "RenderInterface.h"
#include "Materials/MaterialInstanceKeys.h"
#include "Materials/DefaultMaterialV2.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(void* p_meshRenderer, void* p_null);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->RenderInterface = p_renderInterface;
		p_meshRenderer->OnComponentDetached.Closure = p_meshRenderer;
		p_meshRenderer->OnComponentDetached.Callback = MeshRenderer_free;
		_Utils::Observer_register(&p_mehsRendererInfo->AssociatedComponent->ComponentFreeEvent, &p_meshRenderer->OnComponentDetached);

		_Render::DefaultMaterialV2DrawerAllocInfo l_defaultMaterialV2DrawerAllocInfo{};
		l_defaultMaterialV2DrawerAllocInfo.DefaultMaterial = p_renderInterface->RenderMaterialsInterface.DefaultMaterial;
		l_defaultMaterialV2DrawerAllocInfo.DefaultMaterialV2Instance_InputAssets = p_mehsRendererInfo->DefaultMaterialV2Instance_InputAssets;

		_Render::DefaultMaterialV2Instance_alloc(&p_meshRenderer->DefaultMaterialV2Instance, p_renderInterface, &l_defaultMaterialV2DrawerAllocInfo);

	};

	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject)
	{
		_Render::MaterialInstance_pushUniformBuffer(&p_meshRenderer->DefaultMaterialV2Instance.MaterialInstance, _Render::MATERIALINSTANCE_MODEL_BUFFER, l_meshUniformObject);
	};

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Render::DefaultMaterialV2Instance_free(&l_meshRenderer->DefaultMaterialV2Instance, l_meshRenderer->RenderInterface);
	};
}