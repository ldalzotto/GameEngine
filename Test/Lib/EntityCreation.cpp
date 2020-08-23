#include "EntityCreation.h"

#include "GameEngineApplicationInterface.h"
extern "C"
{
#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/MeshRenderer.h"
}
// #include "ECS/Entity.h"
// #include "ECS/ECSEventQueueT.hpp"
// #include "ECS/ComponentT.hpp"
// 
// #include "ECS_Impl/Components/Transform/TransformComponent.h"
// #include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
// #include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

namespace _GameEngine::_Test
{
	void EntityCreation_createEntity(GameEngineApplicationInterface* p_sandboxApplication, CubeCreationInfo* p_sandboxCubeCreationInfo, ECS_Entity_HANDLE* out_entity, TransformComponent_PTR* out_entitytransform)
	{
		{
			*out_entity = ECS_AllocateEntity(p_sandboxApplication->ECS);
		}

		{
			TransformComponent_PTR l_transformComponent = (TransformComponent_PTR)ECS_Component_Alloc(TRANSFORM_COMPONENT_TYPE, sizeof(TransformComponent));

			TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
			l_transformInitInfo.LocalPosition = p_sandboxCubeCreationInfo->LocalPosition;
			l_transformInitInfo.LocalRotation = p_sandboxCubeCreationInfo->LocalRotation;
			l_transformInitInfo.LocalScale = p_sandboxCubeCreationInfo->LocalScale;
			TransformComponent_init(l_transformComponent, &l_transformInitInfo);
			*out_entitytransform = l_transformComponent;

			ECS_AddComponent(p_sandboxApplication->ECS, *out_entity, &l_transformComponent->Header);
		}

		if (p_sandboxCubeCreationInfo->MeshRendererInitInfo)
		{
			MeshRenderer_PTR l_meshRenderer = (MeshRenderer_PTR)ECS_Component_Alloc(MESHRENDERER_COMPONENT_TYPE, sizeof(MeshRenderer));
			MeshRenderer_init(l_meshRenderer, p_sandboxApplication->RenderInterface, p_sandboxCubeCreationInfo->MeshRendererInitInfo);
			ECS_AddComponent(p_sandboxApplication->ECS, *out_entity, &l_meshRenderer->Header);
		}

#if 0
		if (p_sandboxCubeCreationInfo->WithMeshBound)
		{
			auto l_meshRendererBound = _ECS::ComponentT_alloc<_ECS::MeshRendererBound>();
			auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(out_entity, l_meshRendererBound);
			// _ECS::ECSEventQueue_pushMessage(&p_sandboxApplication->ECS->EventQueue, &l_addComponentMessage);
		}
#endif
	};
}