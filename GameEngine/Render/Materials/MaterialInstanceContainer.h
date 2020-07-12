#pragma once

extern "C"
{
#include "DataStructures/GenericArrayNameMacros.h"
#include "DataStructures/Specifications/SortedVector.h"
}

namespace _GameEngine::_Render
{
	struct Material;
	struct MaterialInstance;
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	typedef Core_GenericArray CORE_VECTOR_NAME(MaterialInstanceHandle);

	struct Material_with_MaterialInstances
	{
		Material* Material;
		CORE_VECTOR_NAME(MaterialInstanceHandle) MaterialInstanceV2;
	};

	typedef Core_SortedVector CORE_SORTEDVECTOR_NAME(Material_with_MaterialInstances);

	struct InstancedMaterialsDataStructure
	{
		CORE_SORTEDVECTOR_NAME(Material_with_MaterialInstances) InstanciatedMaterialsV3;
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