#pragma once

#include <unordered_map>
#include <vector>

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2;
	struct MaterialInstance;
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct MaterialInstanceContainer
	{
		RenderInterface* RenderInterface;
		std::unordered_map<DefaultMaterialV2*, std::vector<MaterialInstance*>> InstanciatedMaterials;
	};

	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer);
	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer);

	void MaterialInstanceContainer_addMaterial(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material);
	void MaterialInstanceContainer_removeMaterial(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material);

	void MaterialInstanceContainer_addMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material, MaterialInstance* p_materialInstance);
	void MaterialInstanceContainer_removeMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, DefaultMaterialV2* p_material, MaterialInstance* p_materialInstance);
}