#pragma once

#include <unordered_map>
#include <vector>

namespace _GameEngine::_Render
{
	struct Material;
	struct MaterialInstance;
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct MaterialInstanceContainer
	{
		RenderInterface* RenderInterface;
		std::unordered_map<Material*, std::vector<MaterialInstance*>> InstanciatedMaterials;
	};

	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer);
	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer);

	void MaterialInstanceContainer_addMaterial(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material);
	void MaterialInstanceContainer_removeMaterial(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material);

	void MaterialInstanceContainer_addMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material, MaterialInstance* p_materialInstance);
	void MaterialInstanceContainer_removeMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material, MaterialInstance* p_materialInstance);
}