#include "EntityCreation.h"

#include "GameEngineApplicationInterface.h"
// #include "ECS/ECS.h"
// #include "ECS/Entity.h"
// #include "ECS/ECSEventQueueT.hpp"
// #include "ECS/ComponentT.hpp"
// 
// #include "ECS_Impl/Components/Transform/TransformComponent.h"
// #include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
// #include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

namespace _GameEngine::_Test
{
	void EntityCreation_createEntity(GameEngineApplicationInterface* p_sandboxApplication, CubeCreationInfo* p_sandboxCubeCreationInfo, _ECS::Entity** out_entity, _ECS::TransformComponent** out_entitytransform)
	{
#if 0
		{
			*out_entity = _ECS::Entity_alloc();
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(out_entity);
			// _ECS::ECSEventQueue_pushMessage(&p_sandboxApplication->ECS->EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::TransformComponent* l_transformComponent = _ECS::ComponentT_alloc<_ECS::TransformComponent>();

			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = p_sandboxCubeCreationInfo->LocalPosition;
			l_transformInitInfo.LocalRotation = p_sandboxCubeCreationInfo->LocalRotation;
			l_transformInitInfo.LocalScale = p_sandboxCubeCreationInfo->LocalScale;
			_ECS::TransformComponent_init(l_transformComponent, &l_transformInitInfo);
			*out_entitytransform = l_transformComponent;

			auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(out_entity, l_transformComponent);
			// _ECS::ECSEventQueue_pushMessage(&p_sandboxApplication->ECS->EventQueue, &l_addComponentMessage);
		}

		if (p_sandboxCubeCreationInfo->MeshRendererInitInfo)
		{
			_ECS::MeshRenderer* l_meshRenderer = _ECS::ComponentT_alloc<_ECS::MeshRenderer>();
			_ECS::MeshRenderer_init(l_meshRenderer, p_sandboxApplication->RenderInterface, p_sandboxCubeCreationInfo->MeshRendererInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(out_entity, l_meshRenderer);
			// _ECS::ECSEventQueue_pushMessage(&p_sandboxApplication->ECS->EventQueue, &l_addComponentMessage);
		}

		if (p_sandboxCubeCreationInfo->WithMeshBound)
		{
			auto l_meshRendererBound = _ECS::ComponentT_alloc<_ECS::MeshRendererBound>();
			auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(out_entity, l_meshRendererBound);
			// _ECS::ECSEventQueue_pushMessage(&p_sandboxApplication->ECS->EventQueue, &l_addComponentMessage);
		}
#endif
	};
}