
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

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, ComponentEvents* p_componentEvents)
	{
		EntityComponentListenerInitInfo l_entityComponentListenerInitInfo{};
		l_entityComponentListenerInitInfo.ComponentEvents = p_componentEvents;
		l_entityComponentListenerInitInfo.FilteredTypes = std::vector<ComponentType>{ MeshRendererType, TransformType };
		l_entityComponentListenerInitInfo.OnEntityAddedCallback = onMeshDrawSystemEntityAdded;
		EntityComponentListener_init(&p_meshDrawSystem->EntityComponentListener, &l_entityComponentListenerInitInfo);
	};

	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem, ComponentEvents* p_componentEvents)
	{

		for (Entity* l_entity : p_meshDrawSystem->EntityComponentListener.FilteredEntities)
		{
			MeshRenderer* l_mesRenderer = (MeshRenderer*)_ECS::Entity_getComponent(l_entity, MeshRendererType);
			_Utils::Vector_eraseElementEquals(l_mesRenderer->Render->MeshDrawStep.MeshedToDraw, &l_mesRenderer->Mesh);
		}

		EntityComponentListener_free(&p_meshDrawSystem->EntityComponentListener, p_componentEvents);
	};
}