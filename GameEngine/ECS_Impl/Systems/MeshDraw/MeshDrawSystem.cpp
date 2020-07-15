
#include "MeshDrawSystem.h"

#include <cstdlib>
#include <ctime>

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

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_gameEngineInterface);

	void meshDrawSystem_onComponentsAttached(void* p_null, Entity* p_entity);
	void meshDrawSystem_onComponentsDetached(void* p_null, Entity* p_entity);

	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->Update.Priority = meshDrawSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.OperationCallback = { MeshDrawSystem_update , NULL};
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		_Core::VectorT_alloc(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, 2);
		_Core::VectorT_pushBack(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, &MeshRendererType);
		_Core::VectorT_pushBack(&p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes, &TransformComponentType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = { meshDrawSystem_onComponentsAttached, (void*)nullptr};
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemoved = { meshDrawSystem_onComponentsDetached, (void*)nullptr};
	};

	void meshDrawSystem_onComponentsAttached(void* p_null, Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = ENTITY_GET_COMPONENT(MeshRenderer, p_entity);
		_Render::MaterialInstanceContainer_addMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);
	};
	
	void meshDrawSystem_onComponentsDetached(void* p_null, Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = ENTITY_GET_COMPONENT(MeshRenderer, p_entity);
		_Render::MaterialInstanceContainer_removeMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_gameEngineInterface)
	{
		_ECS::SystemV2* l_meshDrawSystem = (_ECS::SystemV2*)p_meshDrawSystem;

		for (size_t i = 0; i < l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.Size; i++)
		{
			Entity** l_entity =  _Core::VectorT_at(&l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities, i);

			MeshRenderer* l_mesRenderer = ENTITY_GET_COMPONENT(MeshRenderer, *l_entity);
			TransformComponent* l_transform = ENTITY_GET_COMPONENT(TransformComponent, *l_entity);

			if (l_transform->Transform.UserFlag_HasChanged)
			{
				_Render::ModelProjection l_meshUniform{};
				l_meshUniform.Model = _Math::Transform_getLocalToWorldMatrix(&l_transform->Transform);
				_Render::MaterialInstance_pushUniformBuffer(l_mesRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MODEL_BUFFER, &l_meshUniform);

				l_transform->Transform.UserFlag_HasChanged = false;	
			}
		}
	};
}