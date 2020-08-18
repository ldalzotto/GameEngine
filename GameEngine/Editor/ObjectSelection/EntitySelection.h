#pragma once

#include <stdint.h>

extern "C"
{
#include "v2/_interface/BoxC_def.h"
}
#include "v2/Transform/Transform.hpp"

#include "Physics/World/Collider/BoxCollider.h"

namespace _GameEngine
{
	namespace _Input { struct Input; }
	namespace _Physics { struct PhysicsInterface; }
	namespace _ECS {
		struct ECS; struct Entity; struct TransformComponent; struct CameraSystem; struct Camera;
	}
}

namespace _RenderV2
{
	struct RenderV2Interface;
}

namespace _GameEngineEditor
{
	struct GameEngineEditor;
}

namespace _GameEngineEditor
{
	/* The TransformGizmoPlane is used to project mouse delta position to world space. According to SelectedGizmoType, he is oriented and positioned to fit specific requirements. */
	struct TransformGizmoPlane
	{
		_MathV2::Transform Transform;
		BOXF Box;
		_GameEngine::_Physics::BoxCollider Collider;
	};

	enum SelectedGizmoType : short int
	{
		ARROW,
		ROTATION,
		SCALE
	};

	struct TransformGizmoSelectionState
	{
		SelectedGizmoType SelectedGizmoType;

		/* The selected gizmo refer to the single gizmo entity currently selected. This value can only be one of the trhee Gizmo transform in the TransformGizmo object. */
		_GameEngine::_ECS::TransformComponent* SelectedGizmo;

		/* This is to handle the fact that on any transformations, the TransformGizmoMovementGuidePlane position and rotation is setted only once (at the start).
		This is to avoid chaotic behavior if the entity is moving from another source, or rotation when skewed. */
		bool GuidePlaneRotationSet;
	};

	struct TransformGizmo
	{
		_GameEngine::_ECS::TransformComponent* TransformGizoEntity;

		_GameEngine::_ECS::TransformComponent* RightGizmo;
		_GameEngine::_ECS::TransformComponent* UpGizmo;
		_GameEngine::_ECS::TransformComponent* ForwardGizmo;

		TransformGizmoPlane TransformGizmoMovementGuidePlane;
	};


	struct EntitySelection_CachedStructures
	{
		_GameEngine::_ECS::CameraSystem* CameraSystem;
		_GameEngine::_ECS::Camera* ActiveCamera;
		_GameEngine::_ECS::TransformComponent* ActiveCameraTransform;
	};

	struct EntitySelectionState
	{
		_GameEngine::_ECS::Entity* SelectedEntity;
		EntitySelection_CachedStructures CachedStructures;

		TransformGizmoSelectionState TransformGizmoSelectionState;
	};

	struct EntitySelection
	{
		_GameEngine::_ECS::ECS* ECS;
		_GameEngine::_Input::Input* Input;
		_RenderV2::RenderV2Interface* RenderInterface;
		_GameEngine::_Physics::PhysicsInterface* PhysicsInterface;

		TransformGizmo TransformGizmoV2;
		EntitySelectionState EntitySelectionState;
	};

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor);
	void EntitySelection_update(EntitySelection* p_entitySelection);
}