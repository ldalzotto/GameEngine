#pragma once

#include <stdint.h>

extern "C"
{
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/TransformC_def.h"
#include "ECSEngine/Components/TransformComponent_def.h"
#include "ECSEngine/Components/Camera_def.h"
#include "ECSEngine/Systems/CameraRenderSystem_def.h"
#include "ECS_def.h"
#include "Entity_def.h"
#include "Physics/World/Collider/BoxCollider.h"
#include "Physics/PhysicsInterface.h"
}


namespace _GameEngine
{
	namespace _Input { struct Input; }
}

typedef struct RenderV2Interface_TYP RenderV2Interface;

namespace _GameEngineEditor
{
	struct GameEngineEditor;
}

namespace _GameEngineEditor
{
	/* The TransformGizmoPlane is used to project mouse delta position to world space. According to SelectedGizmoType, he is oriented and positioned to fit specific requirements. */
	struct TransformGizmoPlane
	{
		Transform Transform;
		BoxF Box;
		BoxCollider Collider;
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
		TransformComponent_PTR SelectedGizmo;

		/* This is to handle the fact that on any transformations, the TransformGizmoMovementGuidePlane position and rotation is setted only once (at the start).
		This is to avoid chaotic behavior if the entity is moving from another source, or rotation when skewed. */
		bool GuidePlaneRotationSet;
	};

	struct TransformGizmo
	{
		TransformComponent_PTR TransformGizoEntity;
		
		TransformComponent_PTR RightGizmo;
		TransformComponent_PTR UpGizmo;
		TransformComponent_PTR ForwardGizmo;

		TransformGizmoPlane TransformGizmoMovementGuidePlane;
	};


	struct EntitySelection_CachedStructures
	{
		
		Camera_PTR ActiveCamera;
		TransformComponent_PTR ActiveCameraTransform;
	};

	struct EntitySelectionState
	{
		ECS_Entity_HANDLE SelectedEntity;
		EntitySelection_CachedStructures CachedStructures;

		TransformGizmoSelectionState TransformGizmoSelectionState;
	};

	struct EntitySelection
	{
		ECS* ECS;
		_GameEngine::_Input::Input* Input;
		RenderV2Interface* RenderInterface;
		PhysicsInterface_PTR PhysicsInterface;
		CameraRenderSystem_PTR CameraSystem;

		TransformGizmo TransformGizmoV2;
		EntitySelectionState EntitySelectionState;
	};

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor);
	void EntitySelection_update(EntitySelection* p_entitySelection);
}