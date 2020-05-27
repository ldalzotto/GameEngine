
#include "MeshDrawSystem.h"

#include <cstdlib>
#include <ctime>

#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"


namespace _GameEngine::_ECS
{
	void onMeshDrawSystemEntityAdded(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = (MeshRenderer*)_ECS::Entity_getComponent(p_entity, MeshRendererType)->Child;
		_Render::MeshDrawStep_addMeshDrawCommand(&l_mesRenderer->Render->MeshDrawStep, &l_mesRenderer->MeshDrawCommand);
		
	}

	void onMeshDrawSystemEntityRemoved(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = (MeshRenderer*)_ECS::Entity_getComponent(p_entity, MeshRendererType)->Child;
		_Render::MeshDrawStep_removeMeshDrawCommand(&l_mesRenderer->Render->MeshDrawStep, &l_mesRenderer->MeshDrawCommand);
	}

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, ECS* p_ecs , _Render::Render* p_render)
	{
		p_meshDrawSystem->ECS = p_ecs;
		p_meshDrawSystem->Render = p_render;

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
			MeshRenderer* l_mesRenderer = (MeshRenderer*)_ECS::Entity_getComponent(l_entity, MeshRendererType)->Child;
			Transform* l_transform = (Transform*)_ECS::Entity_getComponent(l_entity, TransformType)->Child;

			_Render::MeshUniformObject l_meshUniform{};
			 l_meshUniform.Model = *_ECS::Transform_getLocalToWorldMatrix(l_transform);
			l_meshUniform.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			l_meshUniform.Project = glm::perspective(glm::radians(45.0f), p_meshDrawSystem->Render->SwapChain.SwapChainInfo.SwapExtend.width / (float)p_meshDrawSystem->Render->SwapChain.SwapChainInfo.SwapExtend.height, 0.1f, 10.0f);
			l_meshUniform.Project[1][1] *= -1;
			MeshRenderer_updateMeshDrawUniform(l_mesRenderer, l_meshUniform);
		}
	};

	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem)
	{
		EntityConfigurableContainer_free(&p_meshDrawSystem->EntityConfigurableContainer, p_meshDrawSystem->ECS);
	};
}