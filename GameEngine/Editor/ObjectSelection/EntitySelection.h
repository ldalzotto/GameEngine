#pragma once

#include <stdint.h>

#include "Math/Box/Box.h"
#include "Math/Transform/Transform.h"

#include "Physics/World/Collider/BoxCollider.h"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _Input { struct Input; }
	namespace _Physics { struct PhysicsInterface; }
	namespace _ECS {
		struct ECS; struct Entity; struct TransformComponent; struct CameraSystem; struct Camera;
	}
}

namespace _GameEngineEditor
{
	struct GameEngineEditor;
}

namespace _GameEngineEditor
{
	struct TransformGizmoPlane
	{
		_GameEngine::_Math::Transform Transform;
		_GameEngine::_Math::Box Box;
		_GameEngine::_Physics::BoxCollider Collider;
	};

	struct TransformGizmoSelectionState
	{
		_GameEngine::_ECS::TransformComponent* SelectedArrow;
	};

	struct TransformGizmo
	{
		_GameEngine::_ECS::TransformComponent* TransformGizoEntity;
		_GameEngine::_ECS::TransformComponent* RightArrow;
		_GameEngine::_ECS::TransformComponent* UpArrow;
		_GameEngine::_ECS::TransformComponent* ForwardArrow;
		TransformGizmoPlane TransformGizmoMovementGuidePlane;
	};

	
	struct EntitySelection_CachedStructures
	{
		_GameEngine::_ECS::CameraSystem* CameraSystem;
		_GameEngine::_ECS::Camera* ActiveCamera;
	};

	struct EntitySelection
	{
		_GameEngine::_ECS::ECS* ECS;
		_GameEngine::_Input::Input* Input;
		_GameEngine::_Render::RenderInterface* RenderInterface;
		_GameEngine::_Physics::PhysicsInterface* PhysicsInterface;

		EntitySelection_CachedStructures CachedStructures;
		TransformGizmo TransformGizmoV2;
		TransformGizmoSelectionState TransformGizmoSelectionState;

		_GameEngine::_ECS::Entity* SelectedEntity;
	};

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor);
	void EntitySelection_update(EntitySelection* p_entitySelection);
}