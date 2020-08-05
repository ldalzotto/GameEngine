#include "MaterialInstanceContainer.h"

#include <stdexcept>

#include "Functional/Sort/ElementSorter.hpp"
#include "Algorithm/Compare/CompareAlgorithmT.hpp"
#include "Log/Log.hpp"
#include "RenderInterface.h"
#include "MaterialInstance.h"
#include "Resources/MaterialResourceProvider.h"

namespace _GameEngine::_Render
{

	bool Material_with_MaterialInstances_equals(Material_with_MaterialInstances* p_left, Material* p_right, void* p_null)
	{
		return p_left->Material == p_right;
	};

	bool MaterialInstanceHandle_equals(MaterialInstance** p_left, MaterialInstance* p_right, void* p_null)
	{
		return *p_left == p_right;
	};

	short Material_with_MaterialInstances_sortCompare(Material_with_MaterialInstances* p_left, Material_with_MaterialInstances* p_right, void* p_null)
	{
		return _Core::SortCompare_sizet_sizet(&p_left->Material->RenderingOrder, &p_right->Material->RenderingOrder);
	};

	void Material_with_MaterialInstances_alloc(Material_with_MaterialInstances* p_materialInstances, Material* p_material)
	{
		p_materialInstances->Material = p_material;
		_Core::VectorT_alloc(&p_materialInstances->MaterialInstanceV2, 0);
	};

	void Material_with_MaterialInstances_free(Material_with_MaterialInstances* p_materialInstances)
	{
		_Core::VectorT_free(&p_materialInstances->MaterialInstanceV2);
	};

	void InstancedMaterialsDataStructure_alloc(InstancedMaterialsDataStructure* p_dataStructure)
	{
		_Core::SortedLinearMapT_alloc(&p_dataStructure->MaterialWithMaterialInstances, 0);
	}

	void InstancedMaterialsDataStructure_free(InstancedMaterialsDataStructure* p_dataStructure)
	{
		auto l_materialWithInstancesIterator = _Core::SortedLinearMapT_buildIterator(&p_dataStructure->MaterialWithMaterialInstances);
		while (_Core::VectorIteratorT_moveNext(&l_materialWithInstancesIterator))
		{
			Material_with_MaterialInstances_free(l_materialWithInstancesIterator.Current);

		}
		_Core::SortedLinearMapT_free(&p_dataStructure->MaterialWithMaterialInstances);
	}

	void InstancedMaterialsDataStructure_deepCopy(InstancedMaterialsDataStructure* p_dataStructure, InstancedMaterialsDataStructure* p_out)
	{

		p_out->MaterialWithMaterialInstances = _Core::SortedLinearMapT_deepCopy(&p_dataStructure->MaterialWithMaterialInstances);
	}

	void InstancedMaterialsDataStructure_addMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material)
	{
		Material_with_MaterialInstances l_emptyMaterialWithMaterialInstance{};
		l_emptyMaterialWithMaterialInstance.Material = p_material;
		Material_with_MaterialInstances* l_createdElement = _Core::SortedLinearMapT_pushBack(&p_dataStructure->MaterialWithMaterialInstances,
				l_emptyMaterialWithMaterialInstance,
				_Core::AsserterT<Material_with_MaterialInstances, Material, void>{Material_with_MaterialInstances_equals, p_material},
				_Core::ElementSorterT<Material_with_MaterialInstances, Material_with_MaterialInstances, void>{Material_with_MaterialInstances_sortCompare, nullptr});

		Material_with_MaterialInstances_alloc(l_createdElement, p_material);
	}

	void InstancedMaterialsDataStructure_removeMaterial(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, RenderInterface* p_renderInterface)
	{
				
		_Core::VectorIteratorT<Material_with_MaterialInstances> l_foundMaterialsIterator = _Core::CompareT_find(_Core::SortedLinearMapT_buildIterator(&p_dataStructure->MaterialWithMaterialInstances),
			_Core::ComparatorT<Material_with_MaterialInstances, Material, void>{Material_with_MaterialInstances_equals, p_material });

		if (!l_foundMaterialsIterator.Current)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("InstancedMaterialsDataStructure_removeMaterial : trying to remove a material that is not indexed."));
		}
		
		Material_with_MaterialInstances* l_foundMateials = l_foundMaterialsIterator.Current;

#ifndef NDEBUG

		if (l_foundMateials->MaterialInstanceV2.Size > 0)
		{
			MYLOG_PUSH(p_renderInterface->MyLog, ::_Core::LogLevel::LOG_WARN, "Potential Memory leak. When Material is disposed, there was still MaterialInstances derived from this Material. Consider disposing MaterialInstance first.");
		}
#endif

		Material_with_MaterialInstances_free(l_foundMateials);
		_Core::SortedLinearMapT_erase(&p_dataStructure->MaterialWithMaterialInstances, l_foundMaterialsIterator.CurrentIndex);
	};

	void InstancedMaterialsDataStructure_addMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		_Core::VectorIteratorT<Material_with_MaterialInstances> l_materialsFoundIterator = _Core::CompareT_find(
			_Core::SortedLinearMapT_buildIterator(&p_dataStructure->MaterialWithMaterialInstances),
			_Core::ComparatorT<Material_with_MaterialInstances, Material, void>{Material_with_MaterialInstances_equals, p_material, nullptr}
		);

		if (!l_materialsFoundIterator.Current)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Trying to instanciated a material where the template material has not already been reigsterd. Make sure to register the Material before instanciating it."));
		}

		Material_with_MaterialInstances* l_foundMaterialWithInstances = l_materialsFoundIterator.Current;
		_Core::VectorT_pushBack(&l_materialsFoundIterator.Current->MaterialInstanceV2, p_materialInstance);
	};

	void InstancedMaterialsDataStructure_removeMaterialInstance(InstancedMaterialsDataStructure* p_dataStructure, Material* p_material, MaterialInstance* p_materialInstance)
	{
		_Core::VectorT<MaterialInstance*>* l_materialInstances = NULL;
		{
			_Core::VectorIteratorT<Material_with_MaterialInstances> l_materialsFoundIterator = _Core::CompareT_find(
				 _Core::SortedLinearMapT_buildIterator(&p_dataStructure->MaterialWithMaterialInstances),
				_Core::ComparatorT<Material_with_MaterialInstances, Material, void>{Material_with_MaterialInstances_equals, p_material}
			);
			if (l_materialsFoundIterator.Current)
			{
				Material_with_MaterialInstances* l_foundMaterialWithInstances = (Material_with_MaterialInstances*)l_materialsFoundIterator.Current;
				l_materialInstances = &l_foundMaterialWithInstances->MaterialInstanceV2;
			}
		}

		{
			_Core::VectorT_eraseCompare
			(
				l_materialInstances,
				_Core::ComparatorT<MaterialInstance*, MaterialInstance, void>{MaterialInstanceHandle_equals, p_materialInstance}
			);
		}
	};


	void MaterialInstanceContainer_alloc(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		InstancedMaterialsDataStructure_alloc(&p_materialInstanceContainer->DataStructure);
	};

	void MaterialInstanceContainer_reAllocGraphicsPipeline(MaterialInstanceContainer* p_materialInstanceContainer)
	{
		_Core::VectorIteratorT<Material_with_MaterialInstances> l_instanciatedMaterialsIterator = _Core::SortedLinearMapT_buildIterator(&p_materialInstanceContainer->DataStructure.MaterialWithMaterialInstances);
		while (_Core::VectorIteratorT_moveNext(&l_instanciatedMaterialsIterator))
		{
			Material_reAllocGraphicsPipeline(l_instanciatedMaterialsIterator.Current->Material, p_materialInstanceContainer->RenderInterface);
		}
	};

	void MaterialInstanceContainer_free(MaterialInstanceContainer* p_materialInstanceContainer)
	{
#ifndef NDEBUG
		if (p_materialInstanceContainer->DataStructure.MaterialWithMaterialInstances.Size > 0)
		{
			MYLOG_PUSH(p_materialInstanceContainer->RenderInterface->MyLog, ::_Core::LogLevel::LOG_WARN, "Potential Memory leak. When the MaterialInstanceContainer is being freed, there was still materials or material instance with it."
				" It is recommended to free material instances manually before freeing the container.");
		}
#endif
		// We still clear materials if there is still instances
		if (p_materialInstanceContainer->DataStructure.MaterialWithMaterialInstances.Size > 0)
		{
			InstancedMaterialsDataStructure l_copy;
			{
				InstancedMaterialsDataStructure_deepCopy(&p_materialInstanceContainer->DataStructure, &l_copy);

				_Core::VectorIteratorT<Material_with_MaterialInstances> l_copiedInstanciatedMaterialsIterator = _Core::SortedLinearMapT_buildIterator(&l_copy.MaterialWithMaterialInstances);
				while (_Core::VectorIteratorT_moveNext(&l_copiedInstanciatedMaterialsIterator))
				{
					Material_with_MaterialInstances* l_materialInstances = l_copiedInstanciatedMaterialsIterator.Current;
					for (size_t j = 0; j < l_materialInstances->MaterialInstanceV2.Size; j++)
					{
						MaterialInstance_free(_Core::VectorT_at(&l_materialInstances->MaterialInstanceV2, j));
					}

					MaterialResourceProvider_ReleaseResource(p_materialInstanceContainer->RenderInterface->ResourceProvidersInterface.MaterialResourceProvider, &l_materialInstances->Material->MaterialUniqueKey);
#ifndef NDEBUG
					if (_Core::CompareT_contains(
						_Core::SortedLinearMapT_buildIterator(&p_materialInstanceContainer->DataStructure.MaterialWithMaterialInstances),
						_Core::ComparatorT<Material_with_MaterialInstances, Material, void> {Material_with_MaterialInstances_equals, l_materialInstances->Material}) )
					{
						MYLOG_PUSH(p_materialInstanceContainer->RenderInterface->MyLog, ::_Core::LogLevel::LOG_WARN, "Memory leak detected. When the MaterialInstanceContainer is being freed, releasing a Material resource didn't induce it's destruction.");
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