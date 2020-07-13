#pragma once

extern "C"
{
#include "DataStructures/Specifications/SortedLinearMap.h"
}

namespace _GameEngine::_Render
{
	struct Material;
	struct MaterialInstance;
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	typedef Core_GenericArray Vector_MaterialInstance;

	struct Material_with_MaterialInstances
	{
		Material* Material;
		Vector_MaterialInstance MaterialInstanceV2;
	};

	struct InstancedMaterialsDataStructure
	{
		Core_SortedLinearMap SortedLinearMap_Material_MaterialInstances;
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