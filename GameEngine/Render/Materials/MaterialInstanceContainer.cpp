#include "MaterialInstanceContainer.h"
#include <algorithm>

#include "Log/Log.h"

#include "RenderInterface.h"
#include "MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"

namespace _GameEngine::_Render
{

	std::unordered_map<DefaultMaterialV2*, std::vector<MaterialInstance*>> materialInstanceContainer_deepCopy(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		std::unordered_map<DefaultMaterialV2*, std::vector<MaterialInstance*>> l_copy;
		for (auto&& l_materialEntry : p_materialInstanceContainer->InstanciatedMaterials)
		{
			l_copy.emplace(l_materialEntry.first, std::vector<MaterialInstance*>(l_materialEntry.second));
		}
		return l_copy;
	};

	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		for (auto&& l_materialEntry : p_materialInstanceContainer->InstanciatedMaterials)
		{
			DefaultMaterial_reAllocGraphicsPipeline(l_materialEntry.first, p_materialInstanceContainer->RenderInterface);
		}
	};

	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer)
	{
#ifndef NDEBUG
		if (p_materialInstanceContainer->InstanciatedMaterials.size() > 0)
		{
			_Log::LogInstance->CoreLogger->warn("Potential Memory leak. When the MaterialInstanceContainer is being freed, there was still materials or material instance with it."
							" It is recommended to free material instances manually before freeing the container.");
		}
#endif

		auto l_materialContainerCopy = materialInstanceContainer_deepCopy(p_materialInstanceContainer);

		for (auto&& l_materialEntry : l_materialContainerCopy)
		{
			for (MaterialInstance* l_materialInstance : l_materialEntry.second)
			{
				MaterialInstance_free(l_materialInstance);
			}

			MaterialResourceProvider_ReleaseResource(p_materialInstanceContainer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &l_materialEntry.first->MaterialUniqueKey);

#ifndef NDEBUG
			if (p_materialInstanceContainer->InstanciatedMaterials.contains(l_materialEntry.first))
			{
				_Log::LogInstance->CoreLogger->warn("Memory leak detected. When the MaterialInstanceContainer is being freed, releasing a Material resource didn't induce it's destruction.");
			}
#endif
		}
	};
	
	void MaterialInstanceContainer_addMaterial(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material)
	{
		p_materialInstanceContainer->InstanciatedMaterials.emplace(p_material, std::vector<MaterialInstance*>());
	};
	
	void MaterialInstanceContainer_removeMaterial(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material)
	{
		p_materialInstanceContainer->InstanciatedMaterials.erase(p_material);
	};

	void MaterialInstanceContainer_addMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material, MaterialInstance* p_materialInstance)
	{
		p_materialInstanceContainer->InstanciatedMaterials.at(p_material).emplace_back(p_materialInstance);
	};

	void MaterialInstanceContainer_removeMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material, MaterialInstance* p_materialInstance)
	{
		std::vector<MaterialInstance*>* l_materialInstances = &p_materialInstanceContainer->InstanciatedMaterials.at(p_material);
		l_materialInstances->erase(std::find(l_materialInstances->begin(), l_materialInstances->end(), p_materialInstance));
	};

}