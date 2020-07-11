#include "MaterialInstanceContainer.h"

#include <stdexcept>

extern "C"
{
#include "Log/LogFormatting.h"
#include "Log/Log.h"
#include "Constants.h"
#include "Functional/Vector/VectorWriter.h"
#include "Functional/Vector/VectorAccessor.h"
}

#include "DataStructures/ElementComparators.h"

#include "RenderInterface.h"
#include "MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"

namespace _GameEngine::_Render
{

	bool Material_with_MaterialInstances_equals(Material_with_MaterialInstances* p_left, Material* p_right)
	{
		return p_left->Material == p_right;
	};

	bool MaterialInstanceHandle_equals(MaterialInstanceHandle* p_left, MaterialInstanceHandle p_right, void* p_null)
	{
		return *p_left == p_right;
	};

	short Material_with_MaterialInstances_sortCompare(Material_with_MaterialInstances* p_left, Material_with_MaterialInstances* p_right)
	{
		return _Core::SizeTSortCompararator(p_left->Material->RenderingOrder, p_right->Material->RenderingOrder);
	};

	bool MaterialInstance_compare(MaterialInstance** p_left, MaterialInstance** p_right)
	{
		return (*p_left) == (*p_right);
	};

	void Material_with_MaterialInstances_alloc(Material_with_MaterialInstances* p_materialInstances, Material* p_material)
	{
		p_materialInstances->Material = p_material;
		Core_Vector_alloc(&p_materialInstances->MaterialInstanceV2, sizeof(MaterialInstance*), 0);
	};

	void Material_with_MaterialInstances_free(Material_with_MaterialInstances* p_materialInstances)
	{
		Core_GenericArray_free(&p_materialInstances->MaterialInstanceV2);
	};



	void InstancedMaterialsDataStructure_alloc(InstancedMaterialsDataStructure* p_dataStructure)
	{
		p_dataStructure->InstanciatedMaterialsV3.alloc(2, Material_with_MaterialInstances_sortCompare);
	}

	void InstancedMaterialsDataStructure_free(InstancedMaterialsDataStructure* p_dataStructure)
	{
		for (size_t i = 0; i < p_dataStructure->InstanciatedMaterialsV3.vector()->size(); i++)
		{
			Material_with_MaterialInstances_free(p_dataStructure->InstanciatedMaterialsV3.vector()->at(i));
		}

		p_dataStructure->InstanciatedMaterialsV3.vector()->free();
	}

	void InstancedMaterialsDataStructure_deepCopy(InstancedMaterialsDataStructure* p_dataStructure, InstancedMaterialsDataStructure* p_out)
	{
		p_dataStructure->InstanciatedMaterialsV3.vector()->deepCopy(p_out->InstanciatedMaterialsV3.vector());
		for (size_t i = 0; i < p_dataStructure->InstanciatedMaterialsV3.vector()->size(); i++)
		{
			Material_with_MaterialInstances* l_materialInstances = p_out->InstanciatedMaterialsV3.vector()->at(i);
			l_materialInstances->MaterialInstanceV2 = Core_GenericArray_deepCopy(&l_materialInstances->MaterialInstanceV2);
		}
	}

	void InstancedMaterialsDataStructure_addMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material)
	{
		if (p_dataStructure->InstanciatedMaterialsV3.vector()->contains(Material_with_MaterialInstances_equals, p_material))
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Key already present."));
		}

		Material_with_MaterialInstances l_materialInstances{};
		Material_with_MaterialInstances_alloc(&l_materialInstances, p_material);
		p_dataStructure->InstanciatedMaterialsV3.push_back(&l_materialInstances);
	}

	void InstancedMaterialsDataStructure_removeMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, RenderInterface* p_renderInterface)
	{
		size_t l_eraseIndex = p_dataStructure->InstanciatedMaterialsV3.vector()->getIndex(Material_with_MaterialInstances_equals, p_material);

#ifndef NDEBUG
		if (p_dataStructure->InstanciatedMaterialsV3.vector()->at(l_eraseIndex)->MaterialInstanceV2.Size > 0)
		{
			MYLOG_PUSH(p_renderInterface->MyLog, LOGLEVEL_WARN, "Potential Memory leak. When Material is disposed, there was still MaterialInstances derived from this Material. Consider disposing MaterialInstance first.");
		}
#endif

		Material_with_MaterialInstances_free(p_dataStructure->InstanciatedMaterialsV3.vector()->at(l_eraseIndex));
		p_dataStructure->InstanciatedMaterialsV3.vector()->erase(l_eraseIndex);
	};

	void InstancedMaterialsDataStructure_addMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		CORE_VECTOR_NAME(MaterialInstanceHandle)* l_materialInstances = &p_dataStructure->InstanciatedMaterialsV3.vector()->get(Material_with_MaterialInstances_equals, p_material)->MaterialInstanceV2;
		l_materialInstances->Functions->Writer->PushBack(l_materialInstances, &p_materialInstance);
	};

	void InstancedMaterialsDataStructure_removeMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		CORE_VECTOR_NAME(MaterialInstanceHandle)* l_materialInstances = &p_dataStructure->InstanciatedMaterialsV3.vector()->get(Material_with_MaterialInstances_equals, p_material)->MaterialInstanceV2;
		Core_Comparator l_materialInstancesComparator; ZEROING(Core_Comparator, &l_materialInstancesComparator);
		l_materialInstancesComparator.ComparedObject = p_materialInstance;
		l_materialInstancesComparator.Function = (Core_comparator_function)MaterialInstanceHandle_equals;
		l_materialInstances->Functions->Writer->EraseCompare(l_materialInstances, &l_materialInstancesComparator);
	};


	void MaterialInstanceContainer_alloc(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		InstancedMaterialsDataStructure_alloc(&p_materialInstanceContainer->DataStructure);
	};

	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer)
	{

		for (size_t i = 0; i < p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.vector()->size(); i++)
		{
			Material_with_MaterialInstances* l_materialInstances = p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.vector()->at(i);
			Material_reAllocGraphicsPipeline(l_materialInstances->Material, p_materialInstanceContainer->RenderInterface);
		}
	};

	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer)
	{
#ifndef NDEBUG
		if (p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.vector()->size() > 0)
		{
			MYLOG_PUSH(p_materialInstanceContainer->RenderInterface->MyLog, LOGLEVEL_WARN, "Potential Memory leak. When the MaterialInstanceContainer is being freed, there was still materials or material instance with it."
				" It is recommended to free material instances manually before freeing the container.");
		}
#endif

		InstancedMaterialsDataStructure l_copy;
		{
			InstancedMaterialsDataStructure_deepCopy(&p_materialInstanceContainer->DataStructure, &l_copy);

			for (size_t i = 0; i < l_copy.InstanciatedMaterialsV3.vector()->size(); i++)
			{
				Material_with_MaterialInstances* l_materialInstances = l_copy.InstanciatedMaterialsV3.vector()->at(i);
				for (size_t j = 0; j < l_materialInstances->MaterialInstanceV2.Size; j++)
				{
					MaterialInstanceHandle*l_materialInstance = (MaterialInstanceHandle*)l_materialInstances->MaterialInstanceV2.Functions->Accessor->At(&l_materialInstances->MaterialInstanceV2, j);
					MaterialInstance_free(l_materialInstance);
				}

				MaterialResourceProvider_ReleaseResource(p_materialInstanceContainer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &l_materialInstances->Material->MaterialUniqueKey);

#ifndef NDEBUG
				if (p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.vector()->contains(Material_with_MaterialInstances_equals, l_materialInstances->Material))
				{
					MYLOG_PUSH(p_materialInstanceContainer->RenderInterface->MyLog, LOGLEVEL_WARN, "Memory leak detected. When the MaterialInstanceContainer is being freed, releasing a Material resource didn't induce it's destruction.");
				}
#endif
			}

			InstancedMaterialsDataStructure_free(&p_materialInstanceContainer->DataStructure);
		}
		InstancedMaterialsDataStructure_free(&l_copy);

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