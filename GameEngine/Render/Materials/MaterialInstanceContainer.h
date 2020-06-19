#pragma once

#include <unordered_map>
#include <vector>
#include "DataStructures/VectorT.h"

namespace _GameEngine::_Render
{
	struct Material;
	struct MaterialInstance;
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct Material_with_MaterialInstances
	{
		Material* Material;
		_Core::VectorT<MaterialInstance*> MaterialInstance;
	};

	struct InstancedMaterialsDataStructure
	{
		_Core::VectorT<Material_with_MaterialInstances> InstanciatedMaterialsV2;
	};

	struct MaterialInstanceContainer
	{
		RenderInterface* RenderInterface;
		InstancedMaterialsDataStructure DataStructure;
	};

	void MaterialInstanceContainer_alloc(MaterialInstanceContainer* p_materialInstanceContainer);
	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer);
	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer);

	void MaterialInstanceContainer_addMaterial(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material);
	void MaterialInstanceContainer_removeMaterial(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material);

	void MaterialInstanceContainer_addMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material, MaterialInstance* p_materialInstance);
	void MaterialInstanceContainer_removeMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material, MaterialInstance* p_materialInstance);
}