#include "MaterialInstanceContainer.h"

#include "Log/Log.h"
#include "Utils/Functional/Comparators.h"

#include "RenderInterface.h"
#include "MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"

namespace _GameEngine::_Render
{

	bool Material_with_MaterialInstances_equals(Material_with_MaterialInstances* p_left, Material* p_right)
	{
		return p_left->Material == p_right;
	};

	short Material_with_MaterialInstances_sortCompare(Material_with_MaterialInstances* p_left, Material_with_MaterialInstances* p_right)
	{
		return _Utils::SizeTCompararator(p_left->Material->RenderingOrder, p_right->Material->RenderingOrder);
	};

	bool MaterialInstance_compare(MaterialInstance** p_left, MaterialInstance** p_right)
	{
		return (*p_left) == (*p_right);
	};

	void Material_with_MaterialInstances_alloc(Material_with_MaterialInstances* p_materialInstances, Material* p_material)
	{
		p_materialInstances->Material = p_material;
		p_materialInstances->MaterialInstance.alloc();
	};

	void Material_with_MaterialInstances_free(Material_with_MaterialInstances* p_materialInstances)
	{
		p_materialInstances->MaterialInstance.free();
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
			Material_with_MaterialInstances* l_materialInstances = p_dataStructure->InstanciatedMaterialsV3.vector()->at(i);
			_Core::VectorT<MaterialInstance*> l_newMaterialInstances;
			l_materialInstances->MaterialInstance.deepCopy(&l_newMaterialInstances);
			l_materialInstances->MaterialInstance = l_newMaterialInstances;
		}
	}

	void InstancedMaterialsDataStructure_addMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material)
	{
		if (p_dataStructure->InstanciatedMaterialsV3.vector()->contains(Material_with_MaterialInstances_equals, p_material))
		{
			throw std::runtime_error("Key already present.");
		}

		Material_with_MaterialInstances l_materialInstances{};
		Material_with_MaterialInstances_alloc(&l_materialInstances, p_material);
		p_dataStructure->InstanciatedMaterialsV3.push_back(&l_materialInstances);
	}

	void InstancedMaterialsDataStructure_removeMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material)
	{
		size_t l_eraseIndex = p_dataStructure->InstanciatedMaterialsV3.vector()->getIndex(Material_with_MaterialInstances_equals, p_material);
		Material_with_MaterialInstances_free(p_dataStructure->InstanciatedMaterialsV3.vector()->at(l_eraseIndex));
		p_dataStructure->InstanciatedMaterialsV3.vector()->erase(l_eraseIndex);
	};

	void InstancedMaterialsDataStructure_addMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		p_dataStructure->InstanciatedMaterialsV3.vector()->get(Material_with_MaterialInstances_equals, p_material)->MaterialInstance.push_back(&p_materialInstance);
	};

	void InstancedMaterialsDataStructure_removeMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		 p_dataStructure->InstanciatedMaterialsV3.vector()->get(Material_with_MaterialInstances_equals, p_material)->MaterialInstance.erase(MaterialInstance_compare, &p_materialInstance);
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
			_Log::LogInstance->CoreLogger->warn("Potential Memory leak. When the MaterialInstanceContainer is being freed, there was still materials or material instance with it."
				" It is recommended to free material instances manually before freeing the container.");
		}
#endif

		InstancedMaterialsDataStructure l_copy;
		{
			InstancedMaterialsDataStructure_deepCopy(&p_materialInstanceContainer->DataStructure, &l_copy);

			for (size_t i = 0; i < l_copy.InstanciatedMaterialsV3.vector()->size(); i++)
			{
				Material_with_MaterialInstances* l_materialInstances = l_copy.InstanciatedMaterialsV3.vector()->at(i);
				for (size_t j = 0; j < l_materialInstances->MaterialInstance.size(); j++)
				{
					MaterialInstance** l_materialInstance = l_materialInstances->MaterialInstance.at(j);
					MaterialInstance_free(l_materialInstance);
				}

				MaterialResourceProvider_ReleaseResource(p_materialInstanceContainer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &l_materialInstances->Material->MaterialUniqueKey);

#ifndef NDEBUG
				if (p_materialInstanceContainer->DataStructure.InstanciatedMaterialsV3.vector()->contains(Material_with_MaterialInstances_equals, l_materialInstances->Material))
				{
					_Log::LogInstance->CoreLogger->warn("Memory leak detected. When the MaterialInstanceContainer is being freed, releasing a Material resource didn't induce it's destruction.");
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
		InstancedMaterialsDataStructure_removeMaterial(&p_materialInstanceContainer->DataStructure, p_material);
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