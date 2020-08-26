#include "EntityCreation.h"

#include "GameEngineApplicationInterface.h"

#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/MeshRenderer.h"
#include "ECSEngine/Components/PhysicsBody.h"


void EntityCreation_createEntity(GameEngineApplicationInterface* p_sandboxApplication, CubeCreationInfo* p_sandboxCubeCreationInfo, ECS_Entity_HANDLE* out_entity, TransformComponent_PTR* out_entitytransform)
{
	{
		*out_entity = ECS_AllocateEntity(p_sandboxApplication->ECS);
	}

	{
		TransformComponent_PTR l_transformComponent = ECS_Component_Alloc_TransformComponent();

		TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
		l_transformInitInfo.LocalPosition = p_sandboxCubeCreationInfo->LocalPosition;
		l_transformInitInfo.LocalRotation = p_sandboxCubeCreationInfo->LocalRotation;
		l_transformInitInfo.LocalScale = p_sandboxCubeCreationInfo->LocalScale;
		TransformComponent_init(l_transformComponent, &l_transformInitInfo);
		*out_entitytransform = l_transformComponent;

		ECS_AddComponent_TransformComponent(p_sandboxApplication->ECS, *out_entity, l_transformComponent);
	}

	if (p_sandboxCubeCreationInfo->MeshRendererInitInfo)
	{
		MeshRenderer_PTR l_meshRenderer = ECS_Component_Alloc_MeshRenderer();
		MeshRenderer_init(l_meshRenderer, p_sandboxApplication->RenderInterface, p_sandboxCubeCreationInfo->MeshRendererInitInfo);
		ECS_AddComponent_MeshRenderer(p_sandboxApplication->ECS, *out_entity, l_meshRenderer);
	}

	if (p_sandboxCubeCreationInfo->WithMeshBound)
	{
		PhysicsBody_PTR l_physicsBody = ECS_Component_Alloc_PhysicsBody();
		ECS_AddComponent_PhysicsBody(p_sandboxApplication->ECS, *out_entity, l_physicsBody);
	}
};