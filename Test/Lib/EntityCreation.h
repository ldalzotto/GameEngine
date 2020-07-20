#pragma once

#include "Math/Vector/Vector.h"
#include "Math/Quaternion/Quaternion.h"

namespace _GameEngine
{
	struct GameEngineApplicationInterface;
	namespace _ECS
	{
		struct Entity;
		struct TransformComponent;
		struct MeshRendererInitInfo;
	}
}

namespace _GameEngine::_Test
{

	struct CubeCreationInfo
	{
		_Math::Vector3f LocalPosition;
		_Math::Quaternionf LocalRotation;
		_Math::Vector3f LocalScale;
		bool WithMeshBound;
		_ECS::MeshRendererInitInfo* MeshRendererInitInfo;
	};

	void EntityCreation_createEntity(GameEngineApplicationInterface* p_sandboxApplication, CubeCreationInfo* p_sandboxCubeCreationInfo, _ECS::Entity** out_entity, _ECS::TransformComponent** out_entitytransform);

	// void EntityCreation_crossLayout(GameEngineApplicationInterface* p_sandboxApplication, _ECS::Entity* l_entities[4], _ECS::Entity* l_parent, _Math::Vector2f& l_crossDistance);
}