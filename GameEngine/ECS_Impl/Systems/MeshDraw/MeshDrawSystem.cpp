
#include "MeshDrawSystem.h"

#include <cstdlib>
#include <ctime>

#include "Utils/Algorithm/Algorithm.h"

#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "EngineSequencers/EngineSequencers.h"

#include "RenderInterface.h"
#include "LoopStep/DefaultMaterialDrawStep.h"

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority MeshDrawSystem_getUpdatePriority()
	{
		return UPDATE_PUSH_TO_RENDER_PRIORITY;
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_delta);

	void onMeshDrawSystemEntityAdded(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, p_entity);
		l_mesRenderer->RenderInterface->DefaultMaterialDrawStep->DefaultMaterialV2Instance.emplace_back(&l_mesRenderer->DefaultMaterialV2Instance);
	}

	void onMeshDrawSystemEntityRemoved(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, p_entity);
		_Utils::Vector_eraseElementEquals(l_mesRenderer->RenderInterface->DefaultMaterialDrawStep->DefaultMaterialV2Instance, &l_mesRenderer->DefaultMaterialV2Instance);
	}

	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->Update.Priority = MeshDrawSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.Callback = MeshDrawSystem_update;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = onMeshDrawSystemEntityAdded;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemoved = onMeshDrawSystemEntityRemoved;
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_delta)
	{
		_ECS::SystemV2* l_meshDrawSystem = (_ECS::SystemV2*)p_meshDrawSystem;

		for (size_t i = 0; i < l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			Entity** l_entity = l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.at(i);

			MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, *l_entity);
			Transform* l_transform = GET_COMPONENT(Transform, *l_entity);

			if (l_transform->HasChangedThisFrame)
			{
				_Render::ModelProjection l_meshUniform{};
				l_meshUniform.Model = _ECS::Transform_getLocalToWorldMatrix(l_transform);
				MeshRenderer_updateMeshDrawUniform(l_mesRenderer, &l_meshUniform);

				l_transform->HasChangedThisFrame = false;
			}
		}
	};
}