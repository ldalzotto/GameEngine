#include "MeshRenderer.h"

#include "RenderInterface.h"
#include "Resources/MaterialResourceProvider.h"
#include "Materials/MaterialInstance.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, void* p_null);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->RenderInterface = p_renderInterface;
		p_meshRenderer->MaterialUniqueKey = *p_mehsRendererInfo->MaterialUniqueKey;
		p_meshRenderer->OnComponentDetached = { MeshRenderer_free, p_meshRenderer };
		_Core::ObserverT_register(&p_mehsRendererInfo->AssociatedComponent->OnComponentFree, (_Core::CallbackT<void, void>*) &p_meshRenderer->OnComponentDetached);
		

		_Render::Material* l_material 
				= _Render::MaterialResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.MaterialResourceProvider, &p_meshRenderer->MaterialUniqueKey);

		_Render::MaterialInstanceInitInfo l_materialInstanceInitInfo{};
		l_materialInstanceInitInfo.MaterialInstanceInputParameters = p_mehsRendererInfo->InputParameters;
		l_materialInstanceInitInfo.SourceMaterial = l_material;

		p_meshRenderer->MaterialInstance = _Render::MaterialInstance_alloc(p_renderInterface, &l_materialInstanceInitInfo);
	};

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, void* p_null)
	{
		_Render::MaterialInstance_free(&p_meshRenderer->MaterialInstance);
		_Render::MaterialResourceProvider_ReleaseResource(p_meshRenderer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &p_meshRenderer->MaterialUniqueKey);
	};
}