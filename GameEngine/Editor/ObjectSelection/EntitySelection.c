#include "EntitySelection.h"

#include <float.h>

#include "GameEngineApplicationInterface.h"
#include "Editor/GameEngineEditor.h"


#include "v2/Math.h"
#include "v2/_interface/Intersection.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/TransformC.h"
#include "v2/_interface/PlaneC.h"
#include "v2/_interface/GeometryUtils.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"
#include "Renderer/Gizmo/Gizmo.h"
#include "RenderV2.h"

#include "Physics/World/Collider/BoxCollider.h"
#include "Physics/World/RayCast.h"
#include "Physics/World/Collider/BoxCollider.h"

#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/Camera.h"
#include "ECSEngine/Components/MeshRenderer.h"
#include "ECSEngine/Components/PhysicsBody.h"
#include "ECSEngine/Systems/CameraRenderSystem.h"


#include "Input/InputV2.h"

#include "Common/Colors/Colors.h"

/*
	The EntitySelection is currently missing features.
	//TODO :
		- Gizmo directions is based on an orthonormal basis. When the Entity is skewed (rotated and it's parent has a nonuniform scale), the coordinate system is no
			 more orthonormal. Thus, arrows directions must be rearranged from calculated local directions of the Entity transform, instead of being fixed.
*/


inline bool EntitySelectionState_isCachedStructureInitialized(EntitySelectionState* p_entitySelectionState) { return p_entitySelectionState->CachedStructures.ActiveCamera; };
inline bool EntitySelectionState_isEntitySelected(EntitySelectionState* p_entitySelectionState) { return p_entitySelectionState->SelectedEntity; };


void TransformGizmoV2_alloc(EntitySelection* p_entitySelection, Vector3f_PTR p_initialWorldPosition, ECS* p_ecs);
void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, ECS* p_ecs);
void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(EntitySelection* p_entitySelection, Transform_PTR p_followedTransform);
void TransformGizmo_siwtchGizmoType(EntitySelection* p_entitySelection, SelectedGizmoType p_newGizmoType);
bool TransformGizmo_detemineGizmoTypeSwitch(EntitySelection* p_entitySelection, SelectedGizmoType* p_out);
TransformComponent_PTR TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, Segment_Vector3f_PTR p_collisionRay);
void TransformGizmo_setSelectedGizmo(TransformGizmoSelectionState* p_selectionState, TransformComponent_PTR p_selectedGizmo);
void TransformGizmo_resetState(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState);
bool TransformGizmoSelectionState_anyGizmoSelected(TransformGizmoSelectionState* p_transformGizmoSelectionState);

void entitySelection_detectTheSelectedEntity(EntitySelection* p_entitySelection);
void entitySelection_detectTheSelectedGizmo(EntitySelection* p_entitySelection);

void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, ECS_Entity_HANDLE p_selectedEntity);
void EntitySelection_executeGizmoTransformation(EntitySelection* p_entitySelection);
void EntitySelection_moveSelectedEntity_arrowTranslation(EntitySelection* p_entitySelection);
void EntitySelection_rotateSelectedEntity(EntitySelection* p_entitySelection);
void EntitySelection_scaleSelectedEntity(EntitySelection* p_entitySelection);

void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor)
{
	p_entitySelection->ECS = p_gameEngineEditor->GameEngineApplicationInterface->ECS;
	p_entitySelection->Input = p_gameEngineEditor->GameEngineApplicationInterface->Input;
	p_entitySelection->RenderInterface = p_gameEngineEditor->GameEngineApplicationInterface->RenderInterface;
	p_entitySelection->PhysicsInterface = p_gameEngineEditor->GameEngineApplicationInterface->PhysicsInterface;
	p_entitySelection->CameraSystem = p_gameEngineEditor->GameEngineApplicationInterface->GameEngineApplicationSystems.CameraRenderSystem;
};

void EntitySelection_update(EntitySelection* p_entitySelection)
{
	EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;

	// Set the active camera

	Camera* l_currentCamera = CameraSystem_getCurrentActiveCamera(p_entitySelection->CameraSystem);
	if (l_currentCamera != l_entitySelectionState->CachedStructures.ActiveCamera)
	{
		l_entitySelectionState->CachedStructures.ActiveCamera = l_currentCamera;
		ECS_GetComponent_TransformComponent(l_entitySelectionState->CachedStructures.ActiveCamera->Header.AttachedEntity, &l_entitySelectionState->CachedStructures.ActiveCameraTransform);
	}

	// Trying to detect the selected Entity
	if (!EntitySelectionState_isEntitySelected(l_entitySelectionState))
	{
		if (Input_getState(p_entitySelection->Input, InputKey_MOUSE_BUTTON_1, KeyStateFlag_PRESSED_THIS_FRAME))
		{
			entitySelection_detectTheSelectedEntity(p_entitySelection);
		}
	}
	else
	{
		if (Input_getState(p_entitySelection->Input, InputKey_MOUSE_BUTTON_1, KeyStateFlag_PRESSED_THIS_FRAME))
		{
			entitySelection_detectTheSelectedGizmo(p_entitySelection);

			if (!TransformGizmoSelectionState_anyGizmoSelected(&p_entitySelection->EntitySelectionState.TransformGizmoSelectionState))
			{
				TransformGizmo_resetState(&p_entitySelection->TransformGizmoV2, &l_entitySelectionState->TransformGizmoSelectionState);
				TransformGizmoV2_free(&p_entitySelection->TransformGizmoV2, p_entitySelection->ECS);
				l_entitySelectionState->SelectedEntity = NULL;
			}

		}
		else if (Input_getState(p_entitySelection->Input, InputKey_MOUSE_BUTTON_1, KeyStateFlag_RELEASED_THIS_FRAME))
		{
			TransformGizmo_resetState(&p_entitySelection->TransformGizmoV2, &l_entitySelectionState->TransformGizmoSelectionState);
		}

		if (!TransformGizmoSelectionState_anyGizmoSelected(&p_entitySelection->EntitySelectionState.TransformGizmoSelectionState))
		{
			// Switch gizmo
			SelectedGizmoType l_newSelectedGizmoType;
			if (TransformGizmo_detemineGizmoTypeSwitch(p_entitySelection, &l_newSelectedGizmoType))
			{
				TransformGizmo_siwtchGizmoType(p_entitySelection, l_newSelectedGizmoType);
			}
		}
		else
		{
			// Execute gizmo transformation
			if (Input_getState(p_entitySelection->Input, InputKey_MOUSE_BUTTON_1, KeyStateFlag_PRESSED))
			{
				Vector2d tmp_vec2 = { 0.0, 0.0 };
				if (!Vec_Equals_2d(&p_entitySelection->Input->InputMouse.MouseDelta, &tmp_vec2))
				{
					EntitySelection_executeGizmoTransformation(p_entitySelection);
				}
			}
		}
	}


	// Move the gizmo to follow the selected entity.
	if (EntitySelectionState_isEntitySelected(l_entitySelectionState))
	{
		TransformComponent_PTR l_selectedEntityTransform;
		ECS_GetComponent_TransformComponent(l_entitySelectionState->SelectedEntity, &l_selectedEntityTransform);
		TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(p_entitySelection, &l_selectedEntityTransform->Transform);
		EntitySelection_drawSelectedEntityBoundingBox(p_entitySelection, l_entitySelectionState->SelectedEntity);
	}
}

void entitySelection_detectTheSelectedEntity(EntitySelection* p_entitySelection)
{
	EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
	Vector2f tmp_vec2_0 = { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y };
	Segment_Vector3f l_ray =
		Camera_buildWorldSpaceRay(
			l_entitySelectionState->CachedStructures.ActiveCamera,
			&tmp_vec2_0
		);

	RaycastHit l_hit;
	if (RayCast(p_entitySelection->PhysicsInterface->World, &l_ray.Begin, &l_ray.End, &l_hit))
	{
		Vector3f tmp_vec3;
		TransformComponent_PTR l_transformComponent = TransformComponent_castFromTransform(l_hit.Collider->Transform);
		l_entitySelectionState->SelectedEntity = l_transformComponent->Header.AttachedEntity;
		Transform_GetWorldPosition(&l_transformComponent->Transform, &tmp_vec3);
		TransformGizmoV2_alloc(p_entitySelection, &tmp_vec3, p_entitySelection->ECS);
		ECS_GlobalEvents_ProcessMessages(p_entitySelection->ECS);
	}
};

void entitySelection_detectTheSelectedGizmo(EntitySelection* p_entitySelection)
{
	EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
	Vector2f tmp_vec2_0 = { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y };
	Segment_Vector3f l_ray =
		Camera_buildWorldSpaceRay(
			l_entitySelectionState->CachedStructures.ActiveCamera,
			&tmp_vec2_0
		);

	TransformGizmo_setSelectedGizmo(&l_entitySelectionState->TransformGizmoSelectionState, TransformGizmo_determinedSelectedGizmoComponent(&p_entitySelection->TransformGizmoV2, &l_ray));
};

Segment_Vector3f entitySelection_rayCastMouseDeltaPosition_againstPlane(EntitySelection* p_entitySelection, BoxCollider_PTR p_testedCollider)
{
	Segment_Vector3f l_mouseDelta_worldPosition = { 0 };
	Segment_Vector2f l_mouseDelta_screenPosition = InputMouse_getMouseDeltaScreenPosition(&p_entitySelection->Input->InputMouse);


	Segment_Vector3f l_mouseDelta_begin_ray =
		Camera_buildWorldSpaceRay(
			p_entitySelection->EntitySelectionState.CachedStructures.ActiveCamera,
			&l_mouseDelta_screenPosition.Begin
		);
	Segment_Vector3f l_mouseDelta_end_ray =
		Camera_buildWorldSpaceRay(
			p_entitySelection->EntitySelectionState.CachedStructures.ActiveCamera,
			&l_mouseDelta_screenPosition.End
		);


	BoxCollider_PTR l_raycastedPlane_ptr[1] = { p_testedCollider };
	Array_BoxColliderPTR l_raycastedPlane = { l_raycastedPlane_ptr, 1, 1 };
	RaycastHit l_endHit;
	if (RayCast_Against(&l_raycastedPlane, &l_mouseDelta_end_ray.Begin, &l_mouseDelta_end_ray.End, &l_endHit))
	{
		// _RenderV2::Gizmo::drawPoint(p_entitySelection->RenderInterface->GizmoBuffer, &l_endHit.HitPoint);
		l_mouseDelta_worldPosition.End = *(Vector3f_PTR)&l_endHit.HitPoint;
	}
	else
	{
		return (Segment_Vector3f) { .Begin = Vector3f_ZERO, .End = Vector3f_ZERO };
	}
	RaycastHit l_beginHit;
	if (RayCast_Against(&l_raycastedPlane, &l_mouseDelta_begin_ray.Begin, &l_mouseDelta_begin_ray.End, &l_beginHit))
	{
		// _RenderV2::Gizmo::drawPoint(p_entitySelection->RenderInterface->GizmoBuffer, &l_beginHit.HitPoint);
		l_mouseDelta_worldPosition.Begin = *(Vector3f_PTR)&l_beginHit.HitPoint;
	}
	else
	{
		return (Segment_Vector3f) { .Begin = Vector3f_ZERO, .End = Vector3f_ZERO };
	}


	// Matrix4f tmp_mat4_0;
	// Transform_GetLocalToWorldMatrix(&p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Transform, &tmp_mat4_0);
	// Vector3c l_color = { 255, 255, 255 };
	// Gizmo_DrawBox(p_entitySelection->RenderInterface->GizmoBuffer, &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Box, &tmp_mat4_0, true, &l_color);

	return l_mouseDelta_worldPosition;
}

void EntitySelection_executeGizmoTransformation(EntitySelection* p_entitySelection)
{
	TransformGizmoSelectionState* l_transformGizmoSelectionState = &p_entitySelection->EntitySelectionState.TransformGizmoSelectionState;
	switch (l_transformGizmoSelectionState->SelectedGizmoType)
	{
	case SelectedGizmoType_ARROW:
		EntitySelection_moveSelectedEntity_arrowTranslation(p_entitySelection);
		break;

	case SelectedGizmoType_ROTATION:
		EntitySelection_rotateSelectedEntity(p_entitySelection);
		break;

	case SelectedGizmoType_SCALE:
		EntitySelection_scaleSelectedEntity(p_entitySelection);
		break;
	}
};

void EntitySelection_moveSelectedEntity_arrowTranslation(EntitySelection* p_entitySelection)
{
	Vector3f tmp_vec3_1, tmp_vec3_0; Quaternion4f tmp_quat_1;

	EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
	TransformComponent_PTR l_transformComponent;
	ECS_GetComponent_TransformComponent(l_entitySelectionState->SelectedEntity, &l_transformComponent);
	TransformComponent_PTR l_selectedArrow = l_entitySelectionState->TransformGizmoSelectionState.SelectedGizmo;
	TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

	// We position the world space place
	{
		// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
		Transform_GetWorldPosition(&l_transformComponent->Transform, &tmp_vec3_1);
		Transform_SetWorldPosition(&l_transformGizmoPlane->Transform, &tmp_vec3_1);

		if (!l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet)
		{
			Vector3f l_selectedObject_worldPosition; Transform_GetWorldPosition(&l_transformComponent->Transform, &l_selectedObject_worldPosition);
			Vector3f l_cameraWorldPosition; Transform_GetWorldPosition(&l_entitySelectionState->CachedStructures.ActiveCameraTransform->Transform, &l_cameraWorldPosition);

			Vector3f l_projectedCameraPos_rotationDirection;
			Transform_GetForward(&l_selectedArrow->Transform, &tmp_vec3_0);
			Geom_LookAtPoint_DirectionVector_PerpendicularToNormal(&tmp_vec3_0, &l_selectedObject_worldPosition, &l_cameraWorldPosition, &l_projectedCameraPos_rotationDirection);
			Quat_FromTo((const Vector3f_PTR)&Vector3f_UP, &l_projectedCameraPos_rotationDirection, &tmp_quat_1);

			Transform_SetLocalRotation(&l_transformGizmoPlane->Transform, &tmp_quat_1);
			l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet = true;
		}

		// _MathV2::Matrix4x4<float> tmp_mat4_0;
		// _RenderV2::Gizmo::drawBox(p_entitySelection->RenderInterface->GizmoBuffer, &l_transformGizmoPlane->Box, TransformM::getLocalToWorldMatrix(&l_transformGizmoPlane->Transform, &tmp_mat4_0), true);
	}

	Vector3f l_deltaPosition;
	Segment_Vector3f tmp_seg3_0 = entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider);
	Seg_ToVector_V3F(&tmp_seg3_0, &tmp_vec3_1);
	Transform_GetForward(&l_selectedArrow->Transform, &tmp_vec3_0);
	Vec_Project_3f(&tmp_vec3_1, &tmp_vec3_0, &l_deltaPosition);
	Transform_AddToWorldPosition(&l_transformComponent->Transform, &l_deltaPosition);
}

void EntitySelection_rotateSelectedEntity(EntitySelection* p_entitySelection)
{
	EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
	TransformComponent_PTR l_transformComponent;
	ECS_GetComponent_TransformComponent(l_entitySelectionState->SelectedEntity, &l_transformComponent);
	TransformComponent_PTR l_selectedRotation = l_entitySelectionState->TransformGizmoSelectionState.SelectedGizmo;
	TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

	Vector3f l_guidePlane_worldPosition;
	Transform_GetWorldPosition(&l_transformComponent->Transform, &l_guidePlane_worldPosition);
	Transform_SetWorldPosition(&l_transformGizmoPlane->Transform, &l_guidePlane_worldPosition);

	Vector3f l_rotationAxis_worldSpace;
	if (!l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet)
	{
		//We position the guide plane
		Vector3f l_selectedObject_worldPosition; Transform_GetWorldPosition(&l_transformComponent->Transform, &l_selectedObject_worldPosition);
		Vector3f l_cameraWorldPosition; Transform_GetWorldPosition(&l_entitySelectionState->CachedStructures.ActiveCameraTransform->Transform, &l_cameraWorldPosition);

		Vector3f l_selectedObject_toCamera_direction; Vec_Min_3f_3f(&l_cameraWorldPosition, &l_selectedObject_worldPosition, &l_selectedObject_toCamera_direction);
		Vec_Normalize_3f(&l_selectedObject_toCamera_direction, &l_selectedObject_toCamera_direction);
		Quaternion4f l_planeOritentation; Quat_FromTo((const Vector3f_PTR)&Vector3f_UP, &l_selectedObject_toCamera_direction, &l_planeOritentation);
		Transform_SetLocalRotation(&l_transformGizmoPlane->Transform, &l_planeOritentation);

		l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet = true;
	}

	//calcualte rotation axis, the normal to the plane
	{
		if (l_selectedRotation == p_entitySelection->TransformGizmoV2.RightGizmo)
		{
			Transform_GetRight(&l_transformComponent->Transform, &l_rotationAxis_worldSpace);
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.UpGizmo)
		{
			Transform_GetUp(&l_transformComponent->Transform, &l_rotationAxis_worldSpace);
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.ForwardGizmo)
		{
			Transform_GetForward(&l_transformComponent->Transform, &l_rotationAxis_worldSpace);
		}
	}


	Segment_Vector3f l_deltaPositionDirection_worldSpace = entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider);
	// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_deltaPositionDirection_worldSpace.Begin);
	// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_deltaPositionDirection_worldSpace.End);

	// Perform rotation.



	Vector3f tmp_vec3_0;
	Vector3f l_rotationBegin, l_rotationEnd;

	Vec_Min_3f_3f(&l_deltaPositionDirection_worldSpace.Begin, (Vector3f_PTR)&l_guidePlane_worldPosition, &l_rotationBegin);
	Vec_Normalize_3f(&l_rotationBegin, &l_rotationBegin);
	Vec_Min_3f_3f(&l_deltaPositionDirection_worldSpace.End, (Vector3f_PTR)&l_guidePlane_worldPosition, &l_rotationEnd);
	Vec_Normalize_3f(&l_rotationEnd, &l_rotationEnd);
	float l_deltaRotation = Vec_Angle_Normalized_3f(&l_rotationBegin, &l_rotationEnd);

	// Delta rotation sign
	{
		// The sign of the calculated delta angle is calculated by taking the ray between the camera and the object as reference axis.
		Vector3f l_angleReferenceAxis;
		Transform_GetWorldPosition(&l_entitySelectionState->CachedStructures.ActiveCameraTransform->Transform, &tmp_vec3_0);
		Vec_Min_3f_3f(&tmp_vec3_0, &l_guidePlane_worldPosition, &l_angleReferenceAxis);
		Vec_Normalize_3f(&l_angleReferenceAxis, &l_angleReferenceAxis);
		l_deltaRotation *= Vec_AngleSign_3f(&l_rotationBegin, &l_rotationEnd, &l_angleReferenceAxis);

		// If the rotation axis is not facing the camera, we invert the sign.
		if (Vec_Dot_3f(&l_angleReferenceAxis, (Vector3f_PTR)&l_rotationAxis_worldSpace) <= 0.000f)
		{
			l_deltaRotation *= -1.0f;
		}
	}

	Quaternion4f tmp_quat_0, tmp_quat_1;
	Quaternion4f l_nextRotation;

	Quat_RotateAround((Vector3f_PTR)&l_rotationAxis_worldSpace, l_deltaRotation, &tmp_quat_0);
	Transform_GetWorldRotation(&l_transformComponent->Transform, &tmp_quat_1);
	Quat_Mul(&tmp_quat_0, &tmp_quat_1, &l_nextRotation);

	Transform_SetWorldRotation(&l_transformComponent->Transform, &l_nextRotation);
};

void EntitySelection_scaleSelectedEntity(EntitySelection* p_entitySelection)
{
	Vector3f tmp_vec3_0, tmp_vec3_1; Quaternion4f tmp_quat_1;

	EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
	TransformComponent_PTR l_transformComponent;
	ECS_GetComponent_TransformComponent(l_entitySelectionState->SelectedEntity, &l_transformComponent);
	TransformComponent_PTR l_selectedScale = l_entitySelectionState->TransformGizmoSelectionState.SelectedGizmo;
	TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

	// We position the world space place
	{
		// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
		Transform_GetWorldPosition(&l_transformComponent->Transform, &tmp_vec3_1);
		Transform_SetWorldPosition(&l_transformGizmoPlane->Transform, &tmp_vec3_1);
		if (!l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet)
		{
			Vector3f l_selectedObject_worldPosition; Transform_GetWorldPosition(&l_transformComponent->Transform, &l_selectedObject_worldPosition);
			Vector3f l_cameraWorldPosition; Transform_GetWorldPosition(&l_entitySelectionState->CachedStructures.ActiveCameraTransform->Transform, &l_cameraWorldPosition);

			Vector3f l_projectedCameraPos_rotationDirection;
			Transform_GetForward(&l_selectedScale->Transform, &tmp_vec3_0);
			Geom_LookAtPoint_DirectionVector_PerpendicularToNormal(&tmp_vec3_0, &l_selectedObject_worldPosition, &l_cameraWorldPosition, &l_projectedCameraPos_rotationDirection);
			Quat_FromTo((const Vector3f_PTR)&Vector3f_UP, &l_projectedCameraPos_rotationDirection, &tmp_quat_1);

			Transform_SetLocalRotation(&l_transformGizmoPlane->Transform, &tmp_quat_1);
			l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet = true;
		}
	}

	Vector3f l_selectedScaleForward_localSpace;
	{
		Segment_Vector4f tmp_seg4_0; Segment_Vector3f tmp_seg3_0; Matrix4f tmp_mat4_0;
		Transform_GetForward(&l_selectedScale->Transform, &tmp_vec3_0);
		Segment_Vector4f l_forwardSegment = { .Begin = { 0.0f, 0.0f, 0.0f, 1.0f }, .End = {tmp_vec3_0.x, tmp_vec3_0.y, tmp_vec3_0.z, 1.0f} };
		// ; l_forwardSegment.End.Vec3_w = 1.0f;

		Transform_GetWorldToLocalMatrix(&l_transformComponent->Transform, &tmp_mat4_0);
		Seg_Mul_V4F_M4F(&l_forwardSegment, &tmp_mat4_0, &tmp_seg4_0);
		tmp_seg3_0 = (Segment_Vector3f){ tmp_seg4_0.Begin.Vec3, tmp_seg4_0.End.Vec3 };
		Seg_ToVector_V3F(&tmp_seg3_0, &l_selectedScaleForward_localSpace);
	}

	Vector3f l_deltaScale3D;

	Segment_Vector3f tmp_seg3_0;
	tmp_seg3_0 = entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider);
	Seg_ToVector_V3F(&tmp_seg3_0, &tmp_vec3_1);
	Transform_GetForward(&l_selectedScale->Transform, &tmp_vec3_0);
	Vec_Project_3f(&tmp_vec3_1, &tmp_vec3_0, &l_deltaScale3D);

	float l_scaleSign;
	Transform_GetForward(&l_selectedScale->Transform, &tmp_vec3_0);
	float l_dot = Vec_Dot_3f(&tmp_vec3_0, &l_deltaScale3D);

	if (l_dot >= 0.000f) { l_scaleSign = 1.0f; }
	else { l_scaleSign = -1.0f; }

	float l_scaleLength = Vec_Length_3f(&l_deltaScale3D) * Vec_Length_3f(&l_transformComponent->Transform.LocalScale);
	Vec_Mul_3f_1f(&l_selectedScaleForward_localSpace, l_scaleLength, &l_deltaScale3D);
	Vec_Mul_3f_1f(&l_deltaScale3D, l_scaleSign, &l_deltaScale3D);

	Vec_Add_3f_3f(&l_transformComponent->Transform.LocalScale, &l_deltaScale3D, &tmp_vec3_1);
	Transform_SetLocalScale(&l_transformComponent->Transform, &tmp_vec3_1);
}

void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, ECS_Entity_HANDLE p_selectedEntity)
{
	Matrix4f tmp_mat_0; Vector3c tmp_vec3_0;

	TransformComponent_PTR l_selectedEntityTransform;
	ECS_GetComponent_TransformComponent(p_selectedEntity, &l_selectedEntityTransform);
	PhysicsBody_PTR l_physicsBody;
	ECS_GetComponent_PhysicsBody(p_selectedEntity, &l_physicsBody);

	tmp_vec3_0 = (Vector3c){ (char)255, (char)255, (char)255 };
	Transform_GetLocalToWorldMatrix(&l_selectedEntityTransform->Transform, &tmp_mat_0);

	Gizmo_DrawBox(p_entitySelection->RenderInterface->GizmoBuffer, l_physicsBody->Boxcollider->Box,
		&tmp_mat_0, true, &tmp_vec3_0);
}

TransformComponent_PTR transformGizmoV2_allocArrow(ECS* p_ecs, RenderV2Interface* p_renderInterface, const Vector3c_PTR p_color)
{
	ECS_Entity_HANDLE l_arrowEntity;
	TransformComponent_PTR l_transform;
	{
		l_arrowEntity = ECS_AllocateEntity(p_ecs);
	}
	{
		l_transform = (TransformComponent_PTR)ECS_Component_Alloc(TRANSFORM_COMPONENT_TYPE, sizeof(TransformComponent));
		TransformInitInfo l_transformInitInfo = { 0 };
		l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, 0.0f, 0.0f };
		l_transformInitInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_transformInitInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };

		TransformComponent_init(l_transform, &l_transformInitInfo);
		ECS_AddComponent(p_ecs, l_arrowEntity, &l_transform->Header);
	}
	{
		MeshRenderer_PTR l_meshRenderer = (MeshRenderer_PTR)ECS_Component_Alloc(MESHRENDERER_COMPONENT_TYPE, sizeof(MeshRenderer));
		MeshRendererInitInfo l_meshRendererInitInfo = { 0 };
		l_meshRendererInitInfo.MeshResourcePath = "E:/GameProjects/GameEngine/Assets/Models/ForwardArrow.obj";

		MeshRenderer_init(l_meshRenderer, p_renderInterface, &l_meshRendererInitInfo);
		ECS_AddComponent(p_ecs, l_arrowEntity, &l_meshRenderer->Header);
	}
	/*
	{
		PhysicsBody_PTR l_physicsBody = (PhysicsBody_PTR)ECS_Component_Alloc(PHYSICSBODY_COMPONENT_TYPE, sizeof(PhysicsBody));
		ECS_AddComponent(p_ecs, l_arrowEntity, &l_physicsBody->Header);
	}
	*/

	return l_transform;
}

TransformComponent_PTR transformGizmoV2_allocRotation(ECS* p_ecs, RenderV2Interface* p_renderInterface, const Vector3c_PTR p_color)
{
	ECS_Entity_HANDLE l_rotationEntity;
	TransformComponent_PTR l_transform;
	{
		l_rotationEntity = ECS_AllocateEntity(p_ecs);
	}
	{
		l_transform = (TransformComponent_PTR)ECS_Component_Alloc(TRANSFORM_COMPONENT_TYPE, sizeof(TransformComponent));
		TransformInitInfo l_transformInitInfo = { 0 };
		l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, 0.0f, 0.0f };
		l_transformInitInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_transformInitInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };

		TransformComponent_init(l_transform, &l_transformInitInfo);
		ECS_AddComponent(p_ecs, l_rotationEntity, &l_transform->Header);
	}
	{
		MeshRenderer_PTR l_meshRenderer = (MeshRenderer_PTR)ECS_Component_Alloc(MESHRENDERER_COMPONENT_TYPE, sizeof(MeshRenderer));
		MeshRendererInitInfo l_meshRendererInitInfo = { 0 };
		l_meshRendererInitInfo.MeshResourcePath = "E:/GameProjects/GameEngine/Assets/Models/RotationGizmo.obj";

		MeshRenderer_init(l_meshRenderer, p_renderInterface, &l_meshRendererInitInfo);
		ECS_AddComponent(p_ecs, l_rotationEntity, &l_meshRenderer->Header);
	}
	/*
	{
		PhysicsBody_PTR l_physicsBody = (PhysicsBody_PTR)ECS_Component_Alloc(PHYSICSBODY_COMPONENT_TYPE, sizeof(PhysicsBody));
		ECS_AddComponent(p_ecs, l_rotationEntity, &l_physicsBody->Header);
	}
	*/

	return l_transform;
}

TransformComponent_PTR transformGizmoV2_allocScale(ECS* p_ecs, RenderV2Interface* p_renderInterface, const Vector3c_PTR p_color)
{
	ECS_Entity_HANDLE l_arrowEntity;
	TransformComponent_PTR l_transform;
	{
		l_arrowEntity = ECS_AllocateEntity(p_ecs);
	}
	{
		l_transform = (TransformComponent_PTR)ECS_Component_Alloc(TRANSFORM_COMPONENT_TYPE, sizeof(TransformComponent));
		TransformInitInfo l_transformInitInfo = { 0 };
		l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, 0.0f, 0.0f };
		l_transformInitInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_transformInitInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };

		TransformComponent_init(l_transform, &l_transformInitInfo);
		ECS_AddComponent(p_ecs, l_arrowEntity, &l_transform->Header);
	}
	{
		MeshRenderer_PTR l_meshRenderer = (MeshRenderer_PTR)ECS_Component_Alloc(MESHRENDERER_COMPONENT_TYPE, sizeof(MeshRenderer));
		MeshRendererInitInfo l_meshRendererInitInfo = { 0 };
		l_meshRendererInitInfo.MeshResourcePath = "E:/GameProjects/GameEngine/Assets/Models/ScaleGizmo.obj";

		MeshRenderer_init(l_meshRenderer, p_renderInterface, &l_meshRendererInitInfo);
		ECS_AddComponent(p_ecs, l_arrowEntity, &l_meshRenderer->Header);
	}
	/*
	{
		PhysicsBody_PTR l_physicsBody = (PhysicsBody_PTR)ECS_Component_Alloc(PHYSICSBODY_COMPONENT_TYPE, sizeof(PhysicsBody));
		ECS_AddComponent(p_ecs, l_arrowEntity, &l_physicsBody->Header);
	}
	*/

	return l_transform;
}

void TransformGizmoV2_alloc(EntitySelection* p_entitySelection, Vector3f_PTR p_initialWorldPosition, ECS* p_ecs)
{
	TransformGizmo* p_transformGizmo = &p_entitySelection->TransformGizmoV2;

	if (p_transformGizmo->TransformGizoEntity)
	{
		TransformGizmoV2_free(p_transformGizmo, p_ecs);
	}

	Vector3f tmp_vec3_0; Quaternion4f tmp_quat_0;
	ECS_Entity_HANDLE l_transformGizmo = ECS_AllocateEntity(p_ecs);

	{
		p_transformGizmo->TransformGizoEntity = (TransformComponent_PTR)ECS_Component_Alloc(TRANSFORM_COMPONENT_TYPE, sizeof(TransformComponent));
		TransformInitInfo l_transformInitInfo = { 0 };
		l_transformInitInfo.LocalPosition = *p_initialWorldPosition;
		l_transformInitInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_transformInitInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };

		TransformComponent_init(p_transformGizmo->TransformGizoEntity, &l_transformInitInfo);
		ECS_AddComponent(p_ecs, l_transformGizmo, &p_transformGizmo->TransformGizoEntity->Header);
	}

	switch (p_entitySelection->EntitySelectionState.TransformGizmoSelectionState.SelectedGizmoType)
	{
	case SelectedGizmoType_ARROW:
		//Arrow transform gizmo
	{
		{
			p_transformGizmo->RightGizmo = transformGizmoV2_allocArrow(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_RED);
		}
		{
			p_transformGizmo->UpGizmo = transformGizmoV2_allocArrow(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_GREEN);
		}
		{
			p_transformGizmo->ForwardGizmo = transformGizmoV2_allocArrow(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_BLUE);
		}

		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardGizmo->Transform);
		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightGizmo->Transform);
		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpGizmo->Transform);

		tmp_vec3_0 = (Vector3f){ 0.0f, M_PI * 0.5f, 0.0f };
		Quat_FromEulerAngle(&tmp_vec3_0, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->RightGizmo->Transform, &tmp_quat_0);
		tmp_vec3_0 = (Vector3f){ -M_PI * 0.5f, 0.0f, 0.0f };
		Quat_FromEulerAngle(&tmp_vec3_0, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->UpGizmo->Transform, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->ForwardGizmo->Transform, (Quaternion4f_PTR)&Quaternion4f_IDENTITY);
	}
	break;
	case SelectedGizmoType_ROTATION:
		// Rotation gizmo
	{
		p_transformGizmo->RightGizmo = transformGizmoV2_allocRotation(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_RED);
		p_transformGizmo->UpGizmo = transformGizmoV2_allocRotation(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_GREEN);
		p_transformGizmo->ForwardGizmo = transformGizmoV2_allocRotation(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_BLUE);

		tmp_vec3_0 = (Vector3f){ 0.0f, M_PI * 0.5f, 0.0f };
		Quat_FromEulerAngle(&tmp_vec3_0, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->ForwardGizmo->Transform, &tmp_quat_0);
		tmp_vec3_0 = (Vector3f){ 0.0f, 0.0f, -M_PI * 0.5f };
		Quat_FromEulerAngle(&tmp_vec3_0, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->UpGizmo->Transform, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->RightGizmo->Transform, (Quaternion4f_PTR)&Quaternion4f_IDENTITY);

		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightGizmo->Transform);
		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpGizmo->Transform);
		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardGizmo->Transform);
	}
	break;
	case SelectedGizmoType_SCALE:
		// Scale gizmo
	{
		{
			p_transformGizmo->RightGizmo = transformGizmoV2_allocScale(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_RED);
		}
		{
			p_transformGizmo->UpGizmo = transformGizmoV2_allocScale(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_GREEN);
		}
		{
			p_transformGizmo->ForwardGizmo = transformGizmoV2_allocScale(p_ecs, p_entitySelection->RenderInterface, (const Vector3c_PTR)&Color_BLUE);
		}

		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightGizmo->Transform);
		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpGizmo->Transform);
		Transform_AddChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardGizmo->Transform);

		tmp_vec3_0 = (Vector3f){ 0.0f, M_PI * 0.5f, 0.0f };
		Quat_FromEulerAngle(&tmp_vec3_0, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->RightGizmo->Transform, &tmp_quat_0);
		tmp_vec3_0 = (Vector3f){ -M_PI * 0.5f, 0.0f, 0.0f };
		Quat_FromEulerAngle(&tmp_vec3_0, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->UpGizmo->Transform, &tmp_quat_0);
		Transform_SetLocalRotation(&p_transformGizmo->ForwardGizmo->Transform, (Quaternion4f_PTR)&Quaternion4f_IDENTITY);
	}
	break;
	}







	//Plane instance
	{
		BoxF l_planeBox;
		l_planeBox.Center = (Vector3f){ 0.0f, 0.0f, 0.0f };
		l_planeBox.Extend = (Vector3f){ FLT_MAX, 0.0f, FLT_MAX };
		// l_planeBox.Extend = (Vector3f){ 2.0f, 0.0f, 2.0f };

		p_transformGizmo->TransformGizmoMovementGuidePlane.Box = l_planeBox;

		p_transformGizmo->TransformGizmoMovementGuidePlane.Collider.Box = &p_transformGizmo->TransformGizmoMovementGuidePlane.Box;
		p_transformGizmo->TransformGizmoMovementGuidePlane.Collider.Transform = &p_transformGizmo->TransformGizmoMovementGuidePlane.Transform;

		Vector3f tmp_vec3 = { 1.0f, 1.0f, 1.0f };
		Transform_SetLocalScale(&p_transformGizmo->TransformGizmoMovementGuidePlane.Transform, &tmp_vec3);
	}
}

void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, ECS* p_ecs)
{
	ECS_FreeEntity(p_ecs, p_transformGizmo->TransformGizoEntity->Header.AttachedEntity);
	*p_transformGizmo = (TransformGizmo){ 0 };
	ECS_GlobalEvents_ProcessMessages(p_ecs);
};

void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(EntitySelection* p_entitySelection, Transform_PTR p_followedTransform)
{
	Vector4f tmp_vec4_0;
	// In order for the transform gimo to always have the same visible size, we fix it's z clip space position.
	{
		TransformComponent_PTR l_transformGizmotransform = p_entitySelection->TransformGizmoV2.TransformGizoEntity;
		if (l_transformGizmotransform)
		{
			Vector3f l_followedWorldPosition; Transform_GetWorldPosition(p_followedTransform, &l_followedWorldPosition);
			Quaternion4f l_followedRotation; Transform_GetWorldRotation(p_followedTransform, &l_followedRotation);

			Vector3f l_transformGizmoWorldPosition;
			{
				Matrix4f l_worldToClipMatrix = Camera_worldToClipMatrix(p_entitySelection->EntitySelectionState.CachedStructures.ActiveCamera);
				Matrix4f l_clipToWorldMatrix; Mat_Inv_M4F(&l_worldToClipMatrix, &l_clipToWorldMatrix);

				Vector4f l_selectedEntityTransformClip;
				tmp_vec4_0.Vec3 = l_followedWorldPosition; tmp_vec4_0.Vec3_w = 1.0f;
				Mat_Mul_M4F_V4F_Homogeneous(&l_worldToClipMatrix, &tmp_vec4_0, &l_selectedEntityTransformClip);
				l_selectedEntityTransformClip.z = 0.985f; //Fixed distance in clip space from near plane.
				Mat_Mul_M4F_V4F_Homogeneous(&l_clipToWorldMatrix, &l_selectedEntityTransformClip, &tmp_vec4_0);
				l_transformGizmoWorldPosition = tmp_vec4_0.Vec3;
			}

			Transform_SetWorldPosition(&l_transformGizmotransform->Transform, &l_transformGizmoWorldPosition);
			Transform_SetLocalRotation(&l_transformGizmotransform->Transform, &l_followedRotation);
		}
	}
};

bool TransformGizmo_detemineGizmoTypeSwitch(EntitySelection* p_entitySelection, SelectedGizmoType* p_out)
{
	if (Input_getState(p_entitySelection->Input, InputKey_T, KeyStateFlag_PRESSED_THIS_FRAME))
	{
		*p_out = SelectedGizmoType_ARROW;
		return true;
	}
	else if (Input_getState(p_entitySelection->Input, InputKey_R, KeyStateFlag_PRESSED_THIS_FRAME))
	{
		*p_out = SelectedGizmoType_ROTATION;
		return true;
	}
	else if (Input_getState(p_entitySelection->Input, InputKey_S, KeyStateFlag_PRESSED_THIS_FRAME))
	{
		*p_out = SelectedGizmoType_SCALE;
		return true;
	}

	return false;
};

void TransformGizmo_siwtchGizmoType(EntitySelection* p_entitySelection, SelectedGizmoType p_newGizmoType)
{
	if (p_entitySelection->EntitySelectionState.TransformGizmoSelectionState.SelectedGizmoType != p_newGizmoType)
	{
		Vector3f l_oldWorldPosition; Transform_GetWorldPosition(&p_entitySelection->TransformGizmoV2.TransformGizoEntity->Transform, &l_oldWorldPosition);
		p_entitySelection->EntitySelectionState.TransformGizmoSelectionState.SelectedGizmoType = p_newGizmoType;
		TransformGizmoV2_alloc(p_entitySelection, &l_oldWorldPosition, p_entitySelection->ECS);
	};
};

TransformComponent_PTR TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, Segment_Vector3f_PTR p_collisionRay)
{
	BoxCollider l_arrowBoxColliders[3];
	BoxCollider_PTR l_transformArrowCollidersPtr[3];
	Array_BoxColliderPTR l_transformArrowColliders = { l_transformArrowCollidersPtr , 3, 3 };
	{
		MeshRenderer_PTR tmp_meshRenderer;
		ECS_GetComponent_MeshRenderer(p_transformGizmo->RightGizmo->Header.AttachedEntity, &tmp_meshRenderer);
		l_arrowBoxColliders[0] = (BoxCollider){ .Transform = &p_transformGizmo->RightGizmo->Transform, .Box = &tmp_meshRenderer->MeshBoundingBox };
		l_transformArrowCollidersPtr[0] = &l_arrowBoxColliders[0];

		ECS_GetComponent_MeshRenderer(p_transformGizmo->UpGizmo->Header.AttachedEntity, &tmp_meshRenderer);
		l_arrowBoxColliders[1] = (BoxCollider){ .Transform = &p_transformGizmo->UpGizmo->Transform, .Box = &tmp_meshRenderer->MeshBoundingBox };
		l_transformArrowCollidersPtr[1] = &l_arrowBoxColliders[1];

		ECS_GetComponent_MeshRenderer(p_transformGizmo->ForwardGizmo->Header.AttachedEntity, &tmp_meshRenderer);
		l_arrowBoxColliders[2] = (BoxCollider){ .Transform = &p_transformGizmo->ForwardGizmo->Transform, .Box = &tmp_meshRenderer->MeshBoundingBox };
		l_transformArrowCollidersPtr[2] = &l_arrowBoxColliders[2];
	}
	RaycastHit l_hit;
	if (RayCast_Against(&l_transformArrowColliders, &p_collisionRay->Begin, &p_collisionRay->End, &l_hit))
	{
		return TransformComponent_castFromTransform(l_hit.Collider->Transform);
	}

	return NULL;
}

void TransformGizmo_setSelectedGizmo(TransformGizmoSelectionState* p_selectionState, TransformComponent_PTR p_selectedGizmo)
{
	Vector3f tmp_vec;
	if (p_selectionState->SelectedGizmo)
	{
		tmp_vec = (Vector3f){ 1.0f, 1.0f, 1.0f };
		Transform_SetLocalScale(&p_selectionState->SelectedGizmo->Transform, &tmp_vec);
	}
	if (p_selectedGizmo)
	{
		tmp_vec = (Vector3f){ 1.2f, 1.2f, 1.2f };
		Transform_SetLocalScale(&p_selectedGizmo->Transform, &tmp_vec);
	}
	p_selectionState->SelectedGizmo = p_selectedGizmo;
}

void TransformGizmo_resetState(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState)
{
	p_selectionState->GuidePlaneRotationSet = false;
	TransformGizmo_setSelectedGizmo(p_selectionState, NULL);
};

bool TransformGizmoSelectionState_anyGizmoSelected(TransformGizmoSelectionState* p_transformGizmoSelectionState)
{
	return p_transformGizmoSelectionState->SelectedGizmo;
};