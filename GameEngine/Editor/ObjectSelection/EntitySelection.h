#pragma once

#include <stdint.h>

#include "v2/Box/Box.hpp"
#include "v2/Transform/Transform.hpp"

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
		_MathV2::Transform Transform;
		_MathV2::Box Box;
		_GameEngine::_Physics::BoxCollider Collider;
	};

	struct TransformGizmoSelectionState
	{
		_GameEngine::_ECS::TransformComponent* SelectedArrow;
		_GameEngine::_ECS::TransformComponent* SelectedRotation;
		_GameEngine::_ECS::TransformComponent* SelectedScale;
	};

	struct TransformGizmo
	{
		_GameEngine::_ECS::TransformComponent* TransformGizoEntity;

		_GameEngine::_ECS::TransformComponent* RightArrow;
		_GameEngine::_ECS::TransformComponent* UpArrow;
		_GameEngine::_ECS::TransformComponent* ForwardArrow;

		_GameEngine::_ECS::TransformComponent* XRotation;
		_GameEngine::_ECS::TransformComponent* YRotation;
		_GameEngine::_ECS::TransformComponent* ZRotation;

		_GameEngine::_ECS::TransformComponent* RightScale;
		_GameEngine::_ECS::TransformComponent* UpScale;
		_GameEngine::_ECS::TransformComponent* ForwardScale;

		TransformGizmoPlane TransformGizmoMovementGuidePlane;
	};


	struct EntitySelection_CachedStructures
	{
		_GameEngine::_ECS::CameraSystem* CameraSystem;
		_GameEngine::_ECS::Camera* ActiveCamera;
		_GameEngine::_ECS::TransformComponent* ActiveCameraTransform;
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