
#include "MeshDrawSystem.h"

#include "Utils/Algorithm/Algorithm.h"

#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"

namespace _GameEngine::_ECS
{
	void onMeshDrawSystemEntityAdded(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = (MeshRenderer*)_ECS::Entity_getComponent(p_entity, MeshRendererType)->Child;
		l_mesRenderer->Render->MeshDrawStep.MeshedToDraw.emplace_back(&l_mesRenderer->Mesh);
	}

	void onMeshDrawSystemEntityRemoved(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = (MeshRenderer*)_ECS::Entity_getComponent(p_entity, MeshRendererType)->Child;
		_Utils::Vector_eraseElementEquals(l_mesRenderer->Render->MeshDrawStep.MeshedToDraw, &l_mesRenderer->Mesh);
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

	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem)
	{
		EntityConfigurableContainer_free(&p_meshDrawSystem->EntityConfigurableContainer, p_meshDrawSystem->ECS);
	};
}