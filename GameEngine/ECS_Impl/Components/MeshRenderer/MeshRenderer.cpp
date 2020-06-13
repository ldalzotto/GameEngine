#include "MeshRenderer.h"

#include "RenderInterface.h"
#include "Resources/MaterialResourceProvider.h"
#include "Materials/MaterialInstanceContainer.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(void* p_meshRenderer, void* p_null);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->RenderInterface = p_renderInterface;
		p_meshRenderer->MaterialUniqueKey = *p_mehsRendererInfo->MaterialUniqueKey;
		p_meshRenderer->OnComponentDetached.Closure = p_meshRenderer;
		p_meshRenderer->OnComponentDetached.Callback = MeshRenderer_free;
		_Utils::Observer_register(&p_mehsRendererInfo->AssociatedComponent->ComponentFreeEvent, &p_meshRenderer->OnComponentDetached);


		_Render::Material* l_material 
				= _Render::MaterialResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.MaterialResourceProvider, &p_meshRenderer->MaterialUniqueKey);

		_Render::MaterialInstanceInitInfo l_materialInstanceInitInfo{};
		l_materialInstanceInitInfo.MaterialInstanceInputParameters = p_mehsRendererInfo->InputParameters;
		l_materialInstanceInitInfo.SourceMaterial = l_material;

		_Render::MaterialInstance_init(&p_meshRenderer->MaterialInstance, p_renderInterface, &l_materialInstanceInitInfo);
		_Render::MaterialInstanceContainer_addMaterialInstance(p_renderInterface->MaterialInstanceContainer, l_material, &p_meshRenderer->MaterialInstance);
	};

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Render::MaterialInstance_free(&l_meshRenderer->MaterialInstance);
		_Render::MaterialInstanceContainer_removeMaterialInstance(l_meshRenderer->RenderInterface->MaterialInstanceContainer, l_meshRenderer->MaterialInstance.SourceMaterial, &l_meshRenderer->MaterialInstance);
		_Render::MaterialResourceProvider_ReleaseResource(l_meshRenderer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &l_meshRenderer->MaterialUniqueKey);
	};
}