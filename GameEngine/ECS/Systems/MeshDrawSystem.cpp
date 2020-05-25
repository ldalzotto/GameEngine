
#include "MeshDrawSystem.h"

#include "Utils/Algorithm/Algorithm.h"

#include "ECS/MeshRenderer/MeshRenderer.h"
#include "ECS/Transform/Transform.h"

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

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, MeshDrawSystemInitInfo* p_meshDrawSystemInitInfo)
	{
		EntityComponentListenerInitInfo l_entityComponentListenerInitInfo{};
		l_entityComponentListenerInitInfo.EntityContainer = p_meshDrawSystemInitInfo->EntityContainer;
		l_entityComponentListenerInitInfo.ComponentEvents = p_meshDrawSystemInitInfo->ComponentEvents;
		l_entityComponentListenerInitInfo.ListenedComponentTypes = std::vector<ComponentType>{ MeshRendererType, TransformType };
		l_entityComponentListenerInitInfo.OnEntityThatMatchesComponentTypesAdded = onMeshDrawSystemEntityAdded;
		l_entityComponentListenerInitInfo.OnEntityThatMatchesComponentTypesRemoved = onMeshDrawSystemEntityRemoved;
		EntityComponentListener_init(&p_meshDrawSystem->EntityComponentListener, &l_entityComponentListenerInitInfo);
	};

	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem, ComponentEvents* p_componentEvents)
	{
		EntityComponentListener_free(&p_meshDrawSystem->EntityComponentListener, p_componentEvents);
	};
}