#pragma once

#include <stdint.h>


#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/TransformC_def.h"
#include "ECSEngine/Components/TransformComponent_def.h"
#include "ECSEngine/Components/Camera_def.h"
#include "ECSEngine/Systems/CameraRenderSystem_def.h"
#include "ECS_def.h"
#include "Entity_def.h"
#include "Physics/World/Collider/BoxCollider.h"
#include "Physics/PhysicsInterface.h"
#include "Input/InputV2.h"


typedef struct RenderV2Interface_TYP RenderV2Interface;
typedef struct GameEngineEditor_TYP GameEngineEditor;

/* The TransformGizmoPlane is used to project mouse delta position to world space. According to SelectedGizmoType, he is oriented and positioned to fit specific requirements. */
typedef struct TransformGizmoPlane_TYP
{
	Transform Transform;
	BoxF Box;
	BoxCollider Collider;
}TransformGizmoPlane, * TransformGizmoPlane_PTR;

typedef short int SelectedGizmoType;
#define SelectedGizmoType_ARROW 0
#define SelectedGizmoType_ROTATION 1
#define SelectedGizmoType_SCALE 2

typedef struct TransformGizmoSelectionState_TYP
{
	SelectedGizmoType SelectedGizmoType;

	/* The selected gizmo refer to the single gizmo entity currently selected. This value can only be one of the trhee Gizmo transform in the TransformGizmo object. */
	TransformComponent_PTR SelectedGizmo;

	/* This is to handle the fact that on any transformations, the TransformGizmoMovementGuidePlane position and rotation is setted only once (at the start).
	This is to avoid chaotic behavior if the entity is moving from another source, or rotation when skewed. */
	bool GuidePlaneRotationSet;
}TransformGizmoSelectionState, * TransformGizmoSelectionState_PTR;

typedef struct TransformGizmo_TYP
{
	TransformComponent_PTR TransformGizoEntity;

	TransformComponent_PTR RightGizmo;
	TransformComponent_PTR UpGizmo;
	TransformComponent_PTR ForwardGizmo;

	TransformGizmoPlane TransformGizmoMovementGuidePlane;
}TransformGizmo, * TransformGizmo_PTR;


typedef struct EntitySelection_CachedStructures_TYP
{

	Camera_PTR ActiveCamera;
	TransformComponent_PTR ActiveCameraTransform;
}EntitySelection_CachedStructures, * EntitySelection_CachedStructures_PTR;

typedef struct EntitySelectionState_TYP
{
	ECS_Entity_HANDLE SelectedEntity;
	EntitySelection_CachedStructures CachedStructures;

	TransformGizmoSelectionState TransformGizmoSelectionState;
}EntitySelectionState, * EntitySelectionState_PTR;

typedef struct EntitySelection_TYP
{
	ECS* ECS;
	Input* Input;
	RenderV2Interface* RenderInterface;
	PhysicsInterface_PTR PhysicsInterface;
	CameraRenderSystem_PTR CameraSystem;

	TransformGizmo TransformGizmoV2;
	EntitySelectionState EntitySelectionState;
}EntitySelection, * EntitySelection_PTR;

void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor);
void EntitySelection_update(EntitySelection* p_entitySelection);