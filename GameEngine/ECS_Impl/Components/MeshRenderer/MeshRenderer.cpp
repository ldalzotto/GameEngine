#include "MeshRenderer.h"

#include "RenderInterface.h"
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

		_Render::MaterialInstanceInitInfo l_materialInstanceInitInfo{};
		l_materialInstanceInitInfo.MaterialInstanceInputParameters = p_mehsRendererInfo->InputParameters;
		l_materialInstanceInitInfo.ShaderParameters = &p_renderInterface->RenderMaterialsInterface.DefaultMaterial->LocalInputParameters.ShaderParameters;
		l_materialInstanceInitInfo.DescriptorPool = &p_renderInterface->RenderMaterialsInterface.DefaultMaterial->LocalInputParameters.DescriptorPool;
		l_materialInstanceInitInfo.DescriptorSetLayout = &p_renderInterface->RenderMaterialsInterface.DefaultMaterial->LocalInputParameters.DescriptorSetLayout;
		l_materialInstanceInitInfo.PipelineLayout = &p_renderInterface->RenderMaterialsInterface.DefaultMaterial->FinalDrawObjects.PipelineLayout;

		_Render::MaterialInstance_init(&p_meshRenderer->MaterialInstance, p_renderInterface, &l_materialInstanceInitInfo);
	};

	/*
	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject)
	{
		_Render::MaterialInstance_pushUniformBuffer(&p_meshRenderer->DefaultMaterialV2Instance.MaterialInstance, _Render::MATERIALINSTANCE_MODEL_BUFFER, l_meshUniformObject);
	};
	*/

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Render::MaterialInstance_free(&l_meshRenderer->MaterialInstance);
	};
}