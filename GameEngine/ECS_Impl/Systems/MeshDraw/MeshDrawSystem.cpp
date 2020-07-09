
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
	_Utils::SortedSequencerPriority MeshDrawSystem_updatePriorityBefore()
	{
		return EDITOR_PRIORITY;
	};

	inline _Utils::SortedSequencerPriority meshDrawSystem_getUpdatePriority()
	{
		return UPDATE_PUSH_TO_RENDER_PRIORITY;
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_gameEngineInterface);

	void meshDrawSystem_onComponentsAttached(Entity* p_entity, void* p_null);
	void meshDrawSystem_onComponentsDetached(Entity* p_entity, void* p_null);

	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->Update.Priority = meshDrawSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.Callback = MeshDrawSystem_update;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformComponentType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = meshDrawSystem_onComponentsAttached;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemoved = meshDrawSystem_onComponentsDetached;
	};

	void meshDrawSystem_onComponentsAttached(Entity* p_entity, void* p_null)
	{
		MeshRenderer* l_mesRenderer = ENTITY_GET_COMPONENT(MeshRenderer, p_entity);
		_Render::MaterialInstanceContainer_addMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);
	};
	
	void meshDrawSystem_onComponentsDetached(Entity* p_entity, void* p_null)
	{
		MeshRenderer* l_mesRenderer = ENTITY_GET_COMPONENT(MeshRenderer, p_entity);
		_Render::MaterialInstanceContainer_removeMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_gameEngineInterface)
	{
		_ECS::SystemV2* l_meshDrawSystem = (_ECS::SystemV2*)p_meshDrawSystem;

		for (size_t i = 0; i < l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			Entity** l_entity = l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.at(i);

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