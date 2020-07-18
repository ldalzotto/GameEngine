
#include "MeshDrawSystem.h"

#include <cstdlib>
#include <ctime>

#include "Algorithm/Compare/CompareAlgorithmT.hpp"

#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "EngineSequencers/EngineSequencers.h"

#include "RenderInterface.h"
#include "Materials/Material.h"
#include "Shader/ShaderParameterKeys.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceContainer.h"

namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority MeshDrawSystem_updatePriorityBefore()
	{
		return EDITOR_PRIORITY;
	};

	inline ::_Core::SortedSequencerPriority meshDrawSystem_getUpdatePriority()
	{
		return UPDATE_PUSH_TO_RENDER_PRIORITY;
	};

	struct MeshDrawSystemOperation
	{
		Entity* Entity;
		TransformComponent* TransformComponent;
		MeshRenderer* MeshRenderer;
	};

	bool MeshDrawSystemOperation_EqualsEntity(MeshDrawSystemOperation* p_left, Entity* p_right, void* p_null)
	{
		return p_left->Entity == p_right;
	}

	struct MeshDrawSystem
	{
		_Core::VectorT<MeshDrawSystemOperation> MeshDrawSystemOperations;
	};

	MeshDrawSystem* MeshDrawSystem_Alloc() 
	{
		MeshDrawSystem* l_meshDrawSystem = (MeshDrawSystem*)malloc(sizeof(MeshDrawSystem));
		_Core::VectorT_alloc(&l_meshDrawSystem->MeshDrawSystemOperations, 10);
		return l_meshDrawSystem;
	}

	void MeshDrawSystem_Free(SystemV2* p_meshDrawSystem)
	{
		MeshDrawSystem* l_meshDrawSystem = (MeshDrawSystem*)p_meshDrawSystem->Child;
		_Core::VectorT_free(&l_meshDrawSystem->MeshDrawSystemOperations);
	}

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_gameEngineInterface);

	void meshDrawSystem_onComponentsAttached(void* p_null, Entity* p_entity);
	void meshDrawSystem_onComponentsDetached(void* p_null, Entity* p_entity);

	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->Update.Priority = meshDrawSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.OperationCallback = { MeshDrawSystem_update , NULL };
		p_systemV2AllocInfo->Child = MeshDrawSystem_Alloc();
		p_systemV2AllocInfo->OnSystemDestroyed = MeshDrawSystem_Free;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		_Core::VectorT_alloc(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, 2);
		_Core::VectorT_pushBack(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, &MeshRendererType);
		_Core::VectorT_pushBack(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, &TransformComponentType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = { meshDrawSystem_onComponentsAttached, p_systemV2AllocInfo->Child };
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemoved = { meshDrawSystem_onComponentsDetached, p_systemV2AllocInfo->Child };
	};

	void meshDrawSystem_onComponentsAttached(void* p_meshDrawSystem, Entity* p_entity)
	{
		MeshDrawSystem* l_meshDrawSystem = (MeshDrawSystem*)p_meshDrawSystem;
		MeshDrawSystemOperation l_meshDrawOperation {};
		l_meshDrawOperation.Entity = p_entity;
		l_meshDrawOperation.TransformComponent = *EntityT_getComponent<TransformComponent>(p_entity);
		l_meshDrawOperation.MeshRenderer = *EntityT_getComponent<MeshRenderer>(p_entity);
		_Core::VectorT_pushBack(&l_meshDrawSystem->MeshDrawSystemOperations, &l_meshDrawOperation);

		_Render::MaterialInstanceContainer_addMaterialInstance(l_meshDrawOperation.MeshRenderer->RenderInterface->MaterialInstanceContainer, 
						l_meshDrawOperation.MeshRenderer->MaterialInstance->SourceMaterial, l_meshDrawOperation.MeshRenderer->MaterialInstance);
	};

	void meshDrawSystem_onComponentsDetached(void* p_meshDrawSystem, Entity* p_entity)
	{
		MeshDrawSystem* l_meshDrawSystem = (MeshDrawSystem*)p_meshDrawSystem;
		_Core::VectorIteratorT<MeshDrawSystemOperation> l_involvedOperation =
			_Core::CompareT_find(_Core::VectorT_buildIterator(&l_meshDrawSystem->MeshDrawSystemOperations), _Core::ComparatorT<MeshDrawSystemOperation, Entity, void>{MeshDrawSystemOperation_EqualsEntity, p_entity});
		{
			MeshRenderer* l_mesRenderer = l_involvedOperation.Current->MeshRenderer;
			_Render::MaterialInstanceContainer_removeMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);
			_Core::VectorT_erase(&l_meshDrawSystem->MeshDrawSystemOperations, l_involvedOperation.CurrentIndex);
		}
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_gameEngineInterface)
	{
		MeshDrawSystem* l_meshDrawSystem = (MeshDrawSystem*)((SystemV2*)p_meshDrawSystem)->Child;

		_Core::VectorIteratorT<MeshDrawSystemOperation> l_operations = _Core::VectorT_buildIterator(&l_meshDrawSystem->MeshDrawSystemOperations);
		while (_Core::VectorIteratorT_moveNext(&l_operations))
		{
			MeshDrawSystemOperation* l_operation = l_operations.Current;
			if (l_operation->TransformComponent->Transform.UserFlag_HasChanged)
			{
				_Render::ModelProjection l_meshUniform{};
				l_meshUniform.Model = _Math::Transform_getLocalToWorldMatrix(&l_operation->TransformComponent->Transform);
				_Render::MaterialInstance_pushUniformBuffer(l_operation->MeshRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MODEL_BUFFER, &l_meshUniform);

				l_operation->TransformComponent->Transform.UserFlag_HasChanged = false;
			}
		}
	};
}