#pragma once

extern "C"
{
#include "v2/_interface/QuaternionC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "ECS_def.h"
#include "Entity_def.h"
#include "ECSEngine/Components/MeshRenderer.h"
#include "ECSEngine/Components/TransformComponent_def.h"
}

typedef struct GameEngineApplicationInterface_TYP GameEngineApplicationInterface;

namespace _GameEngine::_Test
{

	struct CubeCreationInfo
	{
		Vector3f LocalPosition;
		Quaternion4f LocalRotation;
		Vector3f LocalScale;
		bool WithMeshBound;
		MeshRendererInitInfo_PTR MeshRendererInitInfo;
	};

	void EntityCreation_createEntity(GameEngineApplicationInterface* p_sandboxApplication, CubeCreationInfo* p_sandboxCubeCreationInfo, ECS_Entity_HANDLE* out_entity, TransformComponent_PTR* out_entitytransform);

	// void EntityCreation_crossLayout(GameEngineApplicationInterface* p_sandboxApplication, _ECS::Entity* l_entities[4], _ECS::Entity* l_parent, _Math::Vector2f& l_crossDistance);
}