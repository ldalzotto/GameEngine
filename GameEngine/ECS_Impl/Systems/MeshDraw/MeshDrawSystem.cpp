
#include "MeshDrawSystem.h"

#include <cstdlib>
#include <ctime>

#include "Utils/Algorithm/Algorithm.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"

#include "RenderInterface.h"
#include "LoopStep/DefaultMaterialDrawStep.h"

namespace _GameEngine::_ECS
{
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

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, ECS* p_ecs)
	{
		p_meshDrawSystem->ECS = p_ecs;

		EntityConfigurableContainerInitInfo l_entityComponentListenerInitInfo{};
		l_entityComponentListenerInitInfo.ECS = p_ecs;
		l_entityComponentListenerInitInfo.ListenedComponentTypes = std::vector<ComponentType>{ MeshRendererType, TransformType };
		l_entityComponentListenerInitInfo.OnEntityThatMatchesComponentTypesAdded = onMeshDrawSystemEntityAdded;
		l_entityComponentListenerInitInfo.OnEntityThatMatchesComponentTypesRemoved = onMeshDrawSystemEntityRemoved;
		EntityConfigurableContainer_init(&p_meshDrawSystem->EntityConfigurableContainer, &l_entityComponentListenerInitInfo);
	};

	void MeshDrawSystem_update(MeshDrawSystem* p_meshDrawSystem, float p_delta)
	{
		//	AccumulatedTime += p_delta;

		for (Entity*& l_entity : p_meshDrawSystem->EntityConfigurableContainer.FilteredEntities)
		{
			MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, l_entity);
			Transform* l_transform = GET_COMPONENT(Transform, l_entity);

			_Render::ModelProjection l_meshUniform{};
			l_meshUniform.Model = *_ECS::Transform_getLocalToWorldMatrix(l_transform);
			MeshRenderer_updateMeshDrawUniform(l_mesRenderer, &l_meshUniform);
		}
	};

	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem)
	{
		EntityConfigurableContainer_free(&p_meshDrawSystem->EntityConfigurableContainer, p_meshDrawSystem->ECS);
	};
}