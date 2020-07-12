#include "MaterialInstanceContainer.h"

extern "C"
{
#include "Log/LogFormatting.h"
#include "Log/Log.h"
#include "ErrorHandling/Errorhandling.h"
#include "Constants.h"
#include "Algorithm/Compare/CompareAlgorithm.h"
#include "Functional/Vector/VectorWriter.h"
#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorIterator.h"
#include "Functional/Sort/ElementSorter.h"
}

#include "RenderInterface.h"
#include "MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"

namespace _GameEngine::_Render
{

	bool Material_with_MaterialInstances_equals(Material_with_MaterialInstances* p_left, Material_with_MaterialInstances* p_right, void* p_null)
	{
		return p_left->Material == p_right->Material;
	};

	bool MaterialInstanceHandle_equals(MaterialInstanceHandle* p_left, MaterialInstanceHandle p_right, void* p_null)
	{
		return *p_left == p_right;
	};

	short Material_with_MaterialInstances_sortCompare(Material_with_MaterialInstances* p_left, Material_with_MaterialInstances* p_right, void* p_null)
	{
		return Core_sortCompare_sizet_sizet(&p_left->Material->RenderingOrder, &p_right->Material->RenderingOrder);
	};

	void Material_with_MaterialInstances_alloc(Material_with_MaterialInstances* p_materialInstances, Material* p_material)
	{
		p_materialInstances->Material = p_material;
		Core_GenericArray_alloc(&p_materialInstances->MaterialInstanceV2, sizeof(MaterialInstance*), 0);
	};

	void Material_with_MaterialInstances_free(Material_with_MaterialInstances* p_materialInstances)
	{
		Core_GenericArray_free(&p_materialInstances->MaterialInstanceV2);
	};

	void InstancedMaterialsDataStructure_alloc(InstancedMaterialsDataStructure* p_dataStructure)
	{
		Core_SortedLinearMap_alloc(&p_dataStructure->InstanciatedMaterialsV3, sizeof(Material_with_MaterialInstances), 2, (Core_elementSort_function)Material_with_MaterialInstances_sortCompare,
			(Core_Equals_function)Material_with_MaterialInstances_equals);
	}

	void InstancedMaterialsDataStructure_free(InstancedMaterialsDataStructure* p_dataStructure)
	{
		for (size_t i = 0; i < p_dataStructure->InstanciatedMaterialsV3.GenericArray.Size; i++)
		{
			Material_with_MaterialInstances_free((Material_with_MaterialInstances*)Core_GenericArray_at(&p_dataStructure->InstanciatedMaterialsV3.GenericArray, 1));
		}

		Core_SortedLinearMap_free(&p_dataStructure->InstanciatedMaterialsV3);
	}

	void InstancedMaterialsDataStructure_deepCopy(InstancedMaterialsDataStructure* p_dataStructure, InstancedMaterialsDataStructure* p_out)
	{
		p_out->InstanciatedMaterialsV3 = Core_SortedLinearMap_deepCopy(&p_dataStructure->InstanciatedMaterialsV3);
	}

	void InstancedMaterialsDataStructure_addMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material)
	{
		Material_with_MaterialInstances l_materialInstances{};
		l_materialInstances.Material = p_material;

		Material_with_MaterialInstances* l_insertedMaterialInstances = NULL;
		ERR_THROW_MESSAGE(Core_SortedLinearMap_pushBack_realloc(&p_dataStructure->InstanciatedMaterialsV3, &l_materialInstances, (void**)&l_insertedMaterialInstances), "Key already present");
		Material_with_MaterialInstances_alloc(l_insertedMaterialInstances, p_material);
	}

	void InstancedMaterialsDataStructure_removeMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, RenderInterface* p_renderInterface)
	{
		Material_with_MaterialInstances l_key{};
		l_key.Material = p_material;

		Core_VectorIterator l_foundmaterialsIterator;
		if (!Core_SortedLinearMap_find(&p_dataStructure->InstanciatedMaterialsV3, &l_key, &l_foundmaterialsIterator))
		{
			ERR_THROW_MESSAGE(CR_OUT_OF_BOUND, "InstancedMaterialsDataStructure_removeMaterial : trying to remove a material that is not indexed.")
		};

		Material_with_MaterialInstances* l_foundMateials = (Material_with_MaterialInstances*)l_foundmaterialsIterator.Current;

#ifndef NDEBUG

		if (l_foundMateials->MaterialInstanceV2.Size > 0)
		{
			MYLOG_PUSH(p_renderInterface->MyLog, LOGLEVEL_WARN, "Potential Memory leak. When Material is disposed, there was still MaterialInstances derived from this Material. Consider disposing MaterialInstance first.");
		}
#endif

		Material_with_MaterialInstances_free(l_foundMateials);
		ERR_THROW(Core_GenericArray_erase(&p_dataStructure->InstanciatedMaterialsV3.GenericArray, l_foundmaterialsIterator.CurrentIndex));
	};

	void InstancedMaterialsDataStructure_addMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		Material_with_MaterialInstances l_comparedMaterial;
		l_comparedMaterial.Material = p_material;
		Core_VectorIterator l_materialsFoundIterator;
		if (!Core_SortedLinearMap_find(&p_dataStructure->InstanciatedMaterialsV3, &l_comparedMaterial, &l_materialsFoundIterator))
		{
			ERR_THROW_MESSAGE(CR_OUT_OF_BOUND, "Trying to instanciated a material where the template material has not already been reigsterd. Make sure to register the Material before instanciating it.");
		}

		Material_with_MaterialInstances* l_foundMaterialWithInstances = (Material_with_MaterialInstances*)l_materialsFoundIterator.Current;
		CORE_VECTOR_NAME(MaterialInstanceHandle)* l_materialInstances = (CORE_VECTOR_NAME(MaterialInstanceHandle)*) & l_foundMaterialWithInstances->MaterialInstanceV2;
		Core_GenericArray_pushBack_realloc(l_materialInstances, &p_materialInstance);
	};

	void InstancedMaterialsDataStructure_removeMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		CORE_VECTOR_NAME(MaterialInstanceHandle)* l_materialInstances = NULL;
		{
			Material_with_MaterialInstances l_comparedMaterial;
			l_comparedMaterial.Material = p_material;
			Core_VectorIterator l_materialsFoundIterator;
			if (Core_SortedLinearMap_find(&p_dataStructure->InstanciatedMaterialsV3, &l_comparedMaterial, &l_materialsFoundIterator))
			{
				Material_with_MaterialInstances* l_foundMaterialWithInstances = (Material_with_MaterialInstances*)l_materialsFoundIterator.Current;
				l_materialInstances = (CORE_VECTOR_NAME(MaterialInstanceHandle)*) & l_foundMaterialWithInstances->MaterialInstanceV2;
			}
		}

		{
			Core_Comparator l_materialInstancesComparator; ZEROING(Core_Comparator, &l_materialInstancesComparator);
			l_materialInstancesComparator.ComparedObject = p_materialInstance;
			l_materialInstancesComparator.Function = (Core_comparator_function)MaterialInstanceHandle_equals;
			Core_GenericArray_eraseCompare(l_materialInstances, &l_materialInstancesComparator);
		}
	};


	void MaterialInstanceContainer_alloc(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		InstancedMaterialsDataStructure_alloc(&p_materialInstanceContainer->DataStructure);
	};

	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		Core_VectorIterator l_instanciatedMaterialsIterator = Core_GenericArray_buildIterator(&p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.GenericArray);
		while (Core_VectorIterator_moveNext(&l_instanciatedMaterialsIterator))
		{
			Material_with_MaterialInstances* l_materialInstances = (Material_with_MaterialInstances*)l_instanciatedMaterialsIterator.Current;
			Material_reAllocGraphicsPipeline(l_materialInstances->Material, p_materialInstanceContainer->RenderInterface);
		}
	};

	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer)
	{
#ifndef NDEBUG
		if (p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.GenericArray.Size > 0)
		{
			MYLOG_PUSH(p_materialInstanceContainer->RenderInterface->MyLog, LOGLEVEL_WARN, "Potential Memory leak. When the MaterialInstanceContainer is being freed, there was still materials or material instance with it."
				" It is recommended to free material instances manually before freeing the container.");
		}
#endif
		// We still clear materials if there is still instances
		if (p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.GenericArray.Size > 0)
		{
			InstancedMaterialsDataStructure l_copy;
			{
				InstancedMaterialsDataStructure_deepCopy(&p_materialInstanceContainer->DataStructure, &l_copy);

				Core_VectorIterator l_copiedInstanciatedMaterialsIterator = Core_GenericArray_buildIterator(&l_copy.InstanciatedMaterialsV3.GenericArray);
				while (Core_VectorIterator_moveNext(&l_copiedInstanciatedMaterialsIterator))
				{
					Material_with_MaterialInstances* l_materialInstances = (Material_with_MaterialInstances*)l_copiedInstanciatedMaterialsIterator.Current;
					for (size_t j = 0; j < l_materialInstances->MaterialInstanceV2.Size; j++)
					{
						MaterialInstanceHandle* l_materialInstance = (MaterialInstanceHandle*)Core_GenericArray_at(&l_materialInstances->MaterialInstanceV2, j);
						MaterialInstance_free(l_materialInstance);
					}

					MaterialResourceProvider_ReleaseResource(p_materialInstanceContainer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &l_materialInstances->Material->MaterialUniqueKey);
#ifndef NDEBUG
					Material_with_MaterialInstances l_compatedMaterial;
					l_compatedMaterial.Material = l_materialInstances->Material;
					Core_VectorIterator l_foundMaterialsIterator;
					if (Core_SortedLinearMap_find(&p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3, &l_compatedMaterial, &l_foundMaterialsIterator))
					{
						MYLOG_PUSH(p_materialInstanceContainer->RenderInterface->MyLog, LOGLEVEL_WARN, "Memory leak detected. When the MaterialInstanceContainer is being freed, releasing a Material resource didn't induce it's destruction.");
					}
#endif
				}
			}
			InstancedMaterialsDataStructure_free(&l_copy);
		}

		InstancedMaterialsDataStructure_free(&p_materialInstanceContainer->DataStructure);

	};

	void MaterialInstanceContainer_addMaterial(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material)
	{
		InstancedMaterialsDataStructure_addMaterial(&p_materialInstanceContainer->DataStructure, p_material);
	};

	void MaterialInstanceContainer_removeMaterial(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material)
	{
		InstancedMaterialsDataStructure_removeMaterial(&p_materialInstanceContainer->DataStructure, p_material, p_materialInstanceContainer->RenderInterface);
	};

	void MaterialInstanceContainer_addMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material, MaterialInstance* p_materialInstance)
	{
		InstancedMaterialsDataStructure_addMaterialInstance(&p_materialInstanceContainer->DataStructure, p_material, p_materialInstance);
	};

	void MaterialInstanceContainer_removeMaterialInstance(MaterialInstanceContainer* p_materialInstanceContainer, Material* p_material, MaterialInstance* p_materialInstance)
	{
		InstancedMaterialsDataStructure_removeMaterialInstance(&p_materialInstanceContainer->DataStructure, p_material, p_materialInstance);
	};

}