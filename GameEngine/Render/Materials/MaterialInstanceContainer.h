#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "DataStructures/Specifications/SortedLinearMapT.hpp"

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
		_Core::VectorT<MaterialInstance*> MaterialInstanceV2;
	};

	struct InstancedMaterialsDataStructure
	{
		_Core::SortedLinearMapT<Material_with_MaterialInstances> MaterialWithMaterialInstances;
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