#include "EntitySelection.h"

#include <float.h>

#include "Editor/GameEngineEditor.h"
#include "GameEngineApplicationInterface.h"

#include "v2/Math.h"
#include "v2/Segment/SegmentV2Math.hpp"
#include "v2/Intersection/Intersection.h"
#include "v2/Transform/TransformM.hpp"
#include "v2/Quaternion/QuaternionMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

#include <iostream>

#include "Colors/Colors.h"

#include "Input/Input.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"

#include "Physics/PhysicsInterface.h"
#include "Physics/World/RayCast.h"
#include "Physics/World/Collider/BoxCollider.h"

#include "Render/Gizmo/Gizmo.h"
#include "Render/RenderInterface.h"
#include "Render/VulkanObjects/Hardware/Window/Window.h"

using namespace _GameEngine;
using namespace _MathV2;

/*
	The EntitySelection is currently missing features.
	//TODO :
		- Gizmo directions is based on an orthonormal basis. When the Entity is skewed (rotated and it's parent has a nonuniform scale), the coordinate system is no
			 more orthonormal. Thus, arrows directions must be rearranged from calculated local directions of the Entity transform, instead of being fixed.
*/

namespace _GameEngineEditor
{
	inline bool EntitySelectionState_isCachedStructureInitialized(EntitySelectionState* p_entitySelectionState) { return p_entitySelectionState->CachedStructures.ActiveCamera; };
	inline bool EntitySelectionState_isEntitySelected(EntitySelectionState* p_entitySelectionState) { return p_entitySelectionState->SelectedEntity; };


	void EntitySelection_onCameraSystem_enabled(EntitySelection* p_entitySelection, _ECS::CameraSystem** p_cameraSystem);

	void TransformGizmoV2_alloc(EntitySelection* p_entitySelection, Vector3<float>& p_initialWorldPosition, _ECS::ECS* p_ecs);
	void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs);
	void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(_GameEngineEditor::EntitySelection* p_entitySelection, Transform& p_followedTransform);
	void TransformGizmo_siwtchGizmoType(EntitySelection* p_entitySelection, SelectedGizmoType p_newGizmoType);
	bool TransformGizmo_detemineGizmoTypeSwitch(EntitySelection* p_entitySelection, SelectedGizmoType* p_out);
	_ECS::TransformComponent* TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, SegmentV2<3, float>* p_collisionRay);
	void TransformGizmo_setSelectedGizmo(TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedGizmo);
	void TransformGizmo_resetState(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState);
	bool TransformGizmoSelectionState_anyGizmoSelected(TransformGizmoSelectionState* p_transformGizmoSelectionState);

	void entitySelection_detectTheSelectedEntity(EntitySelection* p_entitySelection);
	void entitySelection_detectTheSelectedGizmo(EntitySelection* p_entitySelection);

	void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, _ECS::Entity* p_selectedEntity);
	void EntitySelection_executeGizmoTransformation(_GameEngineEditor::EntitySelection* p_entitySelection);
	void EntitySelection_moveSelectedEntity_arrowTranslation(_GameEngineEditor::EntitySelection* p_entitySelection);
	void EntitySelection_rotateSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection);
	void EntitySelection_scaleSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection);

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor)
	{
		p_entitySelection->ECS = p_gameEngineEditor->GameEngineApplicationInterface->ECS;
		p_entitySelection->Input = p_gameEngineEditor->GameEngineApplicationInterface->Input;
		p_entitySelection->RenderInterface = p_gameEngineEditor->GameEngineApplicationInterface->RenderInterface;
		p_entitySelection->PhysicsInterface = p_gameEngineEditor->GameEngineApplicationInterface->PhysicsInterface;

		_Core::CallbackT < EntitySelection, _ECS::CameraSystem* > l_onCameraSystemEnabled = { EntitySelection_onCameraSystem_enabled , p_entitySelection };
		_ECS::SystemEvents_registerOnSystemAdded(&p_gameEngineEditor->GameEngineApplicationInterface->ECS->SystemEvents, _ECS::CameraSystemKey, (_Core::CallbackT<void, _ECS::SystemHeader*>*) & l_onCameraSystemEnabled);
	};

	void EntitySelection_onCameraSystem_enabled(EntitySelection* p_entitySelection, _ECS::CameraSystem** p_cameraSystem)
	{
		p_entitySelection->EntitySelectionState.CachedStructures.CameraSystem = *p_cameraSystem;
	};

	void EntitySelection_update(EntitySelection* p_entitySelection)
	{
		EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;

		// Set the active camera
		_ECS::Camera* l_currentCamera = _ECS::CameraSystem_getCurrentActiveCamera(l_entitySelectionState->CachedStructures.CameraSystem);
		if (l_currentCamera != l_entitySelectionState->CachedStructures.ActiveCamera)
		{
			l_entitySelectionState->CachedStructures.ActiveCamera = _ECS::CameraSystem_getCurrentActiveCamera(l_entitySelectionState->CachedStructures.CameraSystem);
			l_entitySelectionState->CachedStructures.ActiveCameraTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(l_entitySelectionState->CachedStructures.ActiveCamera->ComponentHeader.AttachedEntity);
		}

		// Trying to detect the selected Entity
		if (!EntitySelectionState_isEntitySelected(l_entitySelectionState))
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				entitySelection_detectTheSelectedEntity(p_entitySelection);
			}
		}
		else
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				entitySelection_detectTheSelectedGizmo(p_entitySelection);

				if (!TransformGizmoSelectionState_anyGizmoSelected(&p_entitySelection->EntitySelectionState.TransformGizmoSelectionState))
				{
					TransformGizmo_resetState(&p_entitySelection->TransformGizmoV2, &l_entitySelectionState->TransformGizmoSelectionState);
					TransformGizmoV2_free(&p_entitySelection->TransformGizmoV2, p_entitySelection->ECS);
					l_entitySelectionState->SelectedEntity = nullptr;
				}

			}
			else if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::RELEASED_THIS_FRAME))
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
				if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED))
				{
					Vector2<double> tmp_vec2 = { 0.0, 0.0 };
					if (!VectorM::equals(&p_entitySelection->Input->InputMouse.MouseDelta, &tmp_vec2))
					{
						EntitySelection_executeGizmoTransformation(p_entitySelection);
					}
				}
			}
		}


		// Move the gizmo to follow the selected entity.
		if (EntitySelectionState_isEntitySelected(l_entitySelectionState))
		{
			_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(l_entitySelectionState->SelectedEntity);
			TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(p_entitySelection, l_selectedEntityTransform->Transform);
			EntitySelection_drawSelectedEntityBoundingBox(p_entitySelection, l_entitySelectionState->SelectedEntity);
		}
	}

	void entitySelection_detectTheSelectedEntity(EntitySelection* p_entitySelection)
	{
		EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
		SegmentV2<3, float> l_ray =
			_ECS::Camera_buildWorldSpaceRay(
				l_entitySelectionState->CachedStructures.ActiveCamera,
				Vector2<float> { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y }
		);

		_Physics::RaycastHit l_hit;
		if (_Physics::RayCast(p_entitySelection->PhysicsInterface->World, l_ray.Begin, l_ray.End, &l_hit))
		{
			Vector3<float> tmp_vec3;
			_ECS::TransformComponent* l_transformComponent = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
			l_entitySelectionState->SelectedEntity = l_transformComponent->ComponentHeader.AttachedEntity;
			TransformGizmoV2_alloc(p_entitySelection, *TransformM::getWorldPosition(&l_transformComponent->Transform, &tmp_vec3), p_entitySelection->ECS);
			_ECS::ECSEventQueue_processMessages(&p_entitySelection->ECS->EventQueue);
		}
	};

	void entitySelection_detectTheSelectedGizmo(EntitySelection* p_entitySelection)
	{
		EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
		SegmentV2<3, float> l_ray =
			_ECS::Camera_buildWorldSpaceRay(
				l_entitySelectionState->CachedStructures.ActiveCamera,
				Vector2<float>	{ (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y }
		);

		TransformGizmo_setSelectedGizmo(&l_entitySelectionState->TransformGizmoSelectionState, TransformGizmo_determinedSelectedGizmoComponent(&p_entitySelection->TransformGizmoV2, &l_ray));
	};

	SegmentV2<3, float> entitySelection_rayCastMouseDeltaPosition_againstPlane(_GameEngineEditor::EntitySelection* p_entitySelection, _Physics::BoxCollider* p_testedCollider)
	{
		SegmentV2<3, float> l_mouseDelta_worldPosition{};
		SegmentV2<2, float> l_mouseDelta_screenPosition = _Input::InputMouse_getMouseDeltaScreenPosition(&p_entitySelection->Input->InputMouse);


		SegmentV2<3, float> l_mouseDelta_begin_ray =
			_ECS::Camera_buildWorldSpaceRay(
				p_entitySelection->EntitySelectionState.CachedStructures.ActiveCamera,
				l_mouseDelta_screenPosition.Begin
			);
		SegmentV2<3, float> l_mouseDelta_end_ray =
			_ECS::Camera_buildWorldSpaceRay(
				p_entitySelection->EntitySelectionState.CachedStructures.ActiveCamera,
				l_mouseDelta_screenPosition.End
			);

		_Physics::BoxCollider* l_raycastedPlane_ptr[1] = { p_testedCollider };
		_Core::ArrayT<_Physics::BoxCollider*> l_raycastedPlane = _Core::ArrayT_fromCStyleArray(l_raycastedPlane_ptr, 1);
		_Physics::RaycastHit l_endHit;
		if (_Physics::RayCast_against(&l_raycastedPlane, l_mouseDelta_end_ray.Begin, l_mouseDelta_end_ray.End, &l_endHit))
		{
			// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_endHit.HitPoint);
			l_mouseDelta_worldPosition.End = l_endHit.HitPoint;
		}
		_Physics::RaycastHit l_beginHit;
		if (_Physics::RayCast_against(&l_raycastedPlane, l_mouseDelta_begin_ray.Begin, l_mouseDelta_begin_ray.End, &l_beginHit))
		{
			// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_beginHit.HitPoint);
			l_mouseDelta_worldPosition.Begin = l_beginHit.HitPoint;
		}


		// Matrix4x4<float> tmp_mat4_0;
		// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Box,
		// 	TransformM::getLocalToWorldMatrix(&p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Transform, &tmp_mat4_0), true);

		return l_mouseDelta_worldPosition;
	}

	void EntitySelection_executeGizmoTransformation(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		TransformGizmoSelectionState* l_transformGizmoSelectionState = &p_entitySelection->EntitySelectionState.TransformGizmoSelectionState;
		switch (l_transformGizmoSelectionState->SelectedGizmoType)
		{
		case SelectedGizmoType::ARROW:
			EntitySelection_moveSelectedEntity_arrowTranslation(p_entitySelection);
			break;

		case SelectedGizmoType::ROTATION:
			EntitySelection_rotateSelectedEntity(p_entitySelection);
			break;

		case SelectedGizmoType::SCALE:
			EntitySelection_scaleSelectedEntity(p_entitySelection);
			break;
		}
	};

	void EntitySelection_moveSelectedEntity_arrowTranslation(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		Vector3<float> tmp_vec3_1, tmp_vec3_0; Quaternion<float> tmp_quat_1;

		EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(l_entitySelectionState->SelectedEntity);
		_ECS::TransformComponent* l_selectedArrow = l_entitySelectionState->TransformGizmoSelectionState.SelectedGizmo;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		// We position the world space place
		{
			// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
			TransformM::setWorldPosition(&l_transformGizmoPlane->Transform, TransformM::getWorldPosition(&l_transformComponent->Transform, &tmp_vec3_1));
			if (!l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet)
			{
				TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&l_selectedArrow->Transform, &tmp_quat_1));
				l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet = true;
			}

			// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &l_transformGizmoPlane->Box, TransformM::getLocalToWorldMatrix_ref(&l_transformGizmoPlane->Transform), true);
		}

		Vector3<float> l_deltaPosition;
		VectorM::project(
			SegmentM::toVector(&entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider), &tmp_vec3_1),
			TransformM::getForward(&l_selectedArrow->Transform, &tmp_vec3_0),
			&l_deltaPosition
		);
		TransformM::addToWorldPosition(&l_transformComponent->Transform, &l_deltaPosition);
	}

	void EntitySelection_rotateSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(l_entitySelectionState->SelectedEntity);
		_ECS::TransformComponent* l_selectedRotation = l_entitySelectionState->TransformGizmoSelectionState.SelectedGizmo;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		Vector3<float> l_guidePlane_worldPosition; TransformM::getWorldPosition(&l_transformComponent->Transform, &l_guidePlane_worldPosition);
		TransformM::setWorldPosition(&l_transformGizmoPlane->Transform, &l_guidePlane_worldPosition);

		Vector3<float> l_rotationAxis_worldSpace;
		if (!l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet)
		{
			//We position the guide plane
			if (l_selectedRotation == p_entitySelection->TransformGizmoV2.RightGizmo)
			{
				Quaternion<float> tmp_quat_0, tmp_quat_1;
				TransformM::setLocalRotation(&l_transformGizmoPlane->Transform,
					QuaternionM::mul(
						&l_transformComponent->Transform.LocalRotation,
						QuaternionM::rotateAround(&FORWARD, -M_PI * 0.5f, &tmp_quat_0),
						&tmp_quat_1
					)
				);
			}
			else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.UpGizmo)
			{
				Quaternion<float> tmp_quat;
				TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&p_entitySelection->TransformGizmoV2.ForwardGizmo->Transform, &tmp_quat));
			}
			else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.ForwardGizmo)
			{
				Quaternion<float> tmp_quat_0, tmp_quat_1;
				TransformM::setLocalRotation(&l_transformGizmoPlane->Transform,
					QuaternionM::mul(
						&l_transformComponent->Transform.LocalRotation,
						QuaternionM::rotateAround(&RIGHT, M_PI * 0.5f, &tmp_quat_0),
						&tmp_quat_1
					)
				);
			}

			l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet = true;
		}

		//calcualte rotation axis, the normal to the plane
		{
			Vector3<float> tmp_vec3_0, tmp_vec3_1;
			l_rotationAxis_worldSpace; VectorM::cross(TransformM::getForward(&l_transformGizmoPlane->Transform, &tmp_vec3_0), TransformM::getRight(&l_transformGizmoPlane->Transform, &tmp_vec3_1), &l_rotationAxis_worldSpace);
		}


		SegmentV2<3, float> l_deltaPositionDirection_worldSpace = entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider);
		_Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_deltaPositionDirection_worldSpace.Begin);
		_Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_deltaPositionDirection_worldSpace.End);

		// Perform rotation.



		Vector3<float> tmp_vec3_0;
		Vector3<float> l_rotationBegin, l_rotationEnd;
		float l_deltaRotation =
			VectorM::angle_normalized(
				VectorM::normalize(VectorM::min(&l_deltaPositionDirection_worldSpace.Begin, &l_guidePlane_worldPosition, &l_rotationBegin), &l_rotationBegin),
				VectorM::normalize(VectorM::min(&l_deltaPositionDirection_worldSpace.End, &l_guidePlane_worldPosition, &l_rotationEnd), &l_rotationEnd)
			);

		// Delta rotation sign
		{
			// The sign of the calculated delta angle is calculated by taking the ray between the camera and the object as reference axis.
			Vector3<float> l_angleReferenceAxis;
			VectorM::normalize(VectorM::min(TransformM::getWorldPosition(&l_entitySelectionState->CachedStructures.ActiveCameraTransform->Transform, &tmp_vec3_0), &l_guidePlane_worldPosition, &l_angleReferenceAxis), &l_angleReferenceAxis);
			l_deltaRotation *= VectorM::angleSign(&l_rotationBegin, &l_rotationEnd, &l_angleReferenceAxis);

			// If the rotation axis is not facing the camera, we invert the sign.
			if (VectorM::dot(&l_angleReferenceAxis, &l_rotationAxis_worldSpace) <= 0.000f)
			{
				l_deltaRotation *= -1.0f;
			}
		}

		Quaternion<float> tmp_quat_0, tmp_quat_1;
		Quaternion<float> l_nextRotation;
		QuaternionM::mul(
			QuaternionM::rotateAround(&l_rotationAxis_worldSpace, l_deltaRotation, &tmp_quat_0),
			TransformM::getWorldRotation(&l_transformComponent->Transform, &tmp_quat_1),
			&l_nextRotation
		);

		TransformM::setWorldRotation(&l_transformComponent->Transform, &l_nextRotation);
	};

	void EntitySelection_scaleSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		Vector3<float> tmp_vec3_0, tmp_vec3_1; Quaternion<float> tmp_quat_1;

		EntitySelectionState* l_entitySelectionState = &p_entitySelection->EntitySelectionState;
		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(l_entitySelectionState->SelectedEntity);
		_ECS::TransformComponent* l_selectedScale = l_entitySelectionState->TransformGizmoSelectionState.SelectedGizmo;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		// We position the world space place
		{
			// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
			TransformM::setWorldPosition(&l_transformGizmoPlane->Transform, TransformM::getWorldPosition(&l_transformComponent->Transform, &tmp_vec3_1));
			if (!l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet)
			{
				TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&l_selectedScale->Transform, &tmp_quat_1));
				l_entitySelectionState->TransformGizmoSelectionState.GuidePlaneRotationSet = true;
			}
		}

		Vector3<float> l_selectedScaleForward_localSpace;
		{
			SegmentV2<4, float> tmp_seg4_0; Matrix4x4<float> tmp_mat4_0;
			SegmentV2<4, float> l_forwardSegment = { {0.0f, 0.0f, 0.0f, 1.0f}, VectorM::cast(TransformM::getForward(&l_selectedScale->Transform, &tmp_vec3_0), 1.0f) };
			SegmentM::toVector(&SegmentM::cast(SegmentM::mul(&l_forwardSegment, TransformM::getWorldToLocalMatrix(&l_transformComponent->Transform, &tmp_mat4_0), &tmp_seg4_0)), &l_selectedScaleForward_localSpace);
		}

		Vector3<float> l_deltaScale3D;
		VectorM::project(
			SegmentM::toVector(&entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider), &tmp_vec3_1),
			TransformM::getForward(&l_selectedScale->Transform, &tmp_vec3_0),
			&l_deltaScale3D
		);

		float l_scaleSign;
		float l_dot = VectorM::dot(
			TransformM::getForward(&l_selectedScale->Transform, &tmp_vec3_0),
			&l_deltaScale3D
		);
		if (l_dot >= 0.000f) { l_scaleSign = 1.0f; }
		else { l_scaleSign = -1.0f; }

		float l_scaleLength = VectorM::length(&l_deltaScale3D);
		VectorM::mul(&l_selectedScaleForward_localSpace, l_scaleLength, &l_deltaScale3D);
		VectorM::mul(&l_deltaScale3D, l_scaleSign, &l_deltaScale3D);

		TransformM::setLocalScale(
			&l_transformComponent->Transform,
			VectorM::add(&l_transformComponent->Transform.LocalScale, &l_deltaScale3D, &tmp_vec3_1)
		);

	}

	void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, _ECS::Entity* p_selectedEntity)
	{
		Matrix4x4<float> tmp_mat_0; Vector3<float> tmp_vec3_0;

		_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_selectedEntity);
		_ECS::MeshRendererBound* l_meshRendererBound = _ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_selectedEntity);
		tmp_vec3_0 = { 1.0f, 1.0f, 1.0f };
		_Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &l_meshRendererBound->BoundingBox,
			TransformM::getLocalToWorldMatrix(&l_selectedEntityTransform->Transform, &tmp_mat_0), true, &tmp_vec3_0);
	}

	_ECS::TransformComponent* transformGizmoV2_allocArrow(_ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface, const Vector4<float>* p_color)
	{
		_ECS::Entity* l_arrowEntity;
		_ECS::TransformComponent* l_transform;
		{
			l_arrowEntity = _ECS::Entity_alloc();
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_arrowEntity);
			_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			l_transform = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, 0.0f, 0.0f };
			l_transformInitInfo.LocalRotation = Quaternionf_Identity;
			l_transformInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };

			_ECS::TransformComponent_init(l_transform, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_arrowEntity, l_transform, p_ecs);
		}
		{
			_ECS::MeshRenderer* l_meshRenderer = _ECS::ComponentT_alloc<_ECS::MeshRenderer>();
			_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
			_Render::MaterialUniqueKey l_materialKey{};
			l_materialKey.FragmentShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoFragment.spv";
			l_materialKey.VertexShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoVertex.spv";
			l_meshRendererInitInfo.MaterialUniqueKey = &l_materialKey;
			l_meshRendererInitInfo.InputParameters = {
				{_Render::MATERIALINSTANCE_MESH_KEY, "E:/GameProjects/GameEngine/Assets/Models/ForwardArrow.obj"},
				{_Render::MATERIALINSTANCE_COLOR, (Vector4<float>*)p_color}
			};

			_ECS::MeshRenderer_init(l_meshRenderer, p_renderInterface, &l_meshRendererInitInfo);
			_ECS::EntityT_addComponentDeferred(l_arrowEntity, l_meshRenderer, p_ecs);
		}
		{
			_ECS::MeshRendererBound* l_meshrendererBound = _ECS::ComponentT_alloc<_ECS::MeshRendererBound>();
			_ECS::EntityT_addComponentDeferred(l_arrowEntity, l_meshrendererBound, p_ecs);
		}

		return l_transform;
	}

	_ECS::TransformComponent* transformGizmoV2_allocRotation(_ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface, const Vector4<float>* p_color)
	{
		_ECS::Entity* l_rotationEntity;
		_ECS::TransformComponent* l_transform;
		{
			l_rotationEntity = _ECS::Entity_alloc();
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rotationEntity);
			_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			l_transform = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, 0.0f, 0.0f };
			l_transformInitInfo.LocalRotation = Quaternionf_Identity;
			l_transformInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };

			_ECS::TransformComponent_init(l_transform, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_rotationEntity, l_transform, p_ecs);
		}
		{
			_ECS::MeshRenderer* l_meshRenderer = _ECS::ComponentT_alloc<_ECS::MeshRenderer>();
			_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
			_Render::MaterialUniqueKey l_materialKey{};
			l_materialKey.FragmentShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoFragment.spv";
			l_materialKey.VertexShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoVertex.spv";
			l_meshRendererInitInfo.MaterialUniqueKey = &l_materialKey;
			l_meshRendererInitInfo.InputParameters = {
				{_Render::MATERIALINSTANCE_MESH_KEY, "E:/GameProjects/GameEngine/Assets/Models/RotationGizmo.obj"},
				{_Render::MATERIALINSTANCE_COLOR, (Vector4<float>*)p_color}
			};

			_ECS::MeshRenderer_init(l_meshRenderer, p_renderInterface, &l_meshRendererInitInfo);
			_ECS::EntityT_addComponentDeferred(l_rotationEntity, l_meshRenderer, p_ecs);
		}
		{
			_ECS::MeshRendererBound* l_meshrendererBound = _ECS::ComponentT_alloc<_ECS::MeshRendererBound>();
			_ECS::EntityT_addComponentDeferred(l_rotationEntity, l_meshrendererBound, p_ecs);
		}

		return l_transform;
	}

	_ECS::TransformComponent* transformGizmoV2_allocScale(_ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface, const Vector4<float>* p_color)
	{
		_ECS::Entity* l_arrowEntity;
		_ECS::TransformComponent* l_transform;
		{
			l_arrowEntity = _ECS::Entity_alloc();
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_arrowEntity);
			_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			l_transform = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, 0.0f, 0.0f };
			l_transformInitInfo.LocalRotation = Quaternionf_Identity;
			l_transformInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };

			_ECS::TransformComponent_init(l_transform, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_arrowEntity, l_transform, p_ecs);
		}
		{
			_ECS::MeshRenderer* l_meshRenderer = _ECS::ComponentT_alloc<_ECS::MeshRenderer>();
			_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
			_Render::MaterialUniqueKey l_materialKey{};
			l_materialKey.FragmentShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoFragment.spv";
			l_materialKey.VertexShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoVertex.spv";
			l_meshRendererInitInfo.MaterialUniqueKey = &l_materialKey;
			l_meshRendererInitInfo.InputParameters = {
				{_Render::MATERIALINSTANCE_MESH_KEY, "E:/GameProjects/GameEngine/Assets/Models/ScaleGizmo.obj"},
				{_Render::MATERIALINSTANCE_COLOR, (Vector4<float>*)p_color}
			};

			_ECS::MeshRenderer_init(l_meshRenderer, p_renderInterface, &l_meshRendererInitInfo);
			_ECS::EntityT_addComponentDeferred(l_arrowEntity, l_meshRenderer, p_ecs);
		}
		{
			_ECS::MeshRendererBound* l_meshrendererBound = _ECS::ComponentT_alloc<_ECS::MeshRendererBound>();
			_ECS::EntityT_addComponentDeferred(l_arrowEntity, l_meshrendererBound, p_ecs);
		}

		return l_transform;
	}

	void TransformGizmoV2_alloc(EntitySelection* p_entitySelection, Vector3<float>& p_initialWorldPosition, _ECS::ECS* p_ecs)
	{
		TransformGizmo* p_transformGizmo = &p_entitySelection->TransformGizmoV2;

		if (p_transformGizmo->TransformGizoEntity)
		{
			TransformGizmoV2_free(p_transformGizmo, p_ecs);
		}

		Vector3<float> tmp_vec3_0; Quaternion<float> tmp_quat_0;
		_ECS::Entity* l_transformGizmo = _ECS::Entity_alloc();
		{
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_transformGizmo);
			_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			p_transformGizmo->TransformGizoEntity = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = p_initialWorldPosition;
			l_transformInitInfo.LocalRotation = Quaternionf_Identity;
			l_transformInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };

			_ECS::TransformComponent_init(p_transformGizmo->TransformGizoEntity, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_transformGizmo, p_transformGizmo->TransformGizoEntity, p_ecs);
		}

		switch (p_entitySelection->EntitySelectionState.TransformGizmoSelectionState.SelectedGizmoType)
		{
		case SelectedGizmoType::ARROW:
			//Arrow transform gizmo
		{
			{
				p_transformGizmo->RightGizmo = transformGizmoV2_allocArrow(p_ecs, p_entitySelection->RenderInterface, &_Color::RED);
			}
			{
				p_transformGizmo->UpGizmo = transformGizmoV2_allocArrow(p_ecs, p_entitySelection->RenderInterface, &_Color::GREEN);
			}
			{
				p_transformGizmo->ForwardGizmo = transformGizmoV2_allocArrow(p_ecs, p_entitySelection->RenderInterface, &_Color::BLUE);
			}

			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardGizmo->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightGizmo->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpGizmo->Transform);

			tmp_vec3_0 = { 0.0f, M_PI * 0.5f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->RightGizmo->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			tmp_vec3_0 = { -M_PI * 0.5f, 0.0f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->UpGizmo->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			TransformM::setLocalRotation(&p_transformGizmo->ForwardGizmo->Transform, &Quaternionf_Identity);
		}
		break;
		case SelectedGizmoType::ROTATION:
			// Rotation gizmo
		{
			p_transformGizmo->RightGizmo = transformGizmoV2_allocRotation(p_ecs, p_entitySelection->RenderInterface, &_Color::RED);
			p_transformGizmo->UpGizmo = transformGizmoV2_allocRotation(p_ecs, p_entitySelection->RenderInterface, &_Color::GREEN);
			p_transformGizmo->ForwardGizmo = transformGizmoV2_allocRotation(p_ecs, p_entitySelection->RenderInterface, &_Color::BLUE);

			tmp_vec3_0 = { 0.0f, M_PI * 0.5f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->ForwardGizmo->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			tmp_vec3_0 = { 0.0f, 0.0f, -M_PI * 0.5f };
			TransformM::setLocalRotation(&p_transformGizmo->UpGizmo->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			TransformM::setLocalRotation(&p_transformGizmo->RightGizmo->Transform, &Quaternionf_Identity);

			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightGizmo->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpGizmo->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardGizmo->Transform);
		}
		break;
		case SelectedGizmoType::SCALE:
			// Scale gizmo
		{
			{
				p_transformGizmo->RightGizmo = transformGizmoV2_allocScale(p_ecs, p_entitySelection->RenderInterface, &_Color::RED);
			}
			{
				p_transformGizmo->UpGizmo = transformGizmoV2_allocScale(p_ecs, p_entitySelection->RenderInterface, &_Color::GREEN);
			}
			{
				p_transformGizmo->ForwardGizmo = transformGizmoV2_allocScale(p_ecs, p_entitySelection->RenderInterface, &_Color::BLUE);
			}

			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightGizmo->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpGizmo->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardGizmo->Transform);

			tmp_vec3_0 = { 0.0f, M_PI * 0.5f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->RightGizmo->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			tmp_vec3_0 = { -M_PI * 0.5f, 0.0f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->UpGizmo->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			TransformM::setLocalRotation(&p_transformGizmo->ForwardGizmo->Transform, &Quaternionf_Identity);
		}
		break;
		}







		//Plane instance
		{
			Box l_planeBox;
			l_planeBox.Center = { 0.0f, 0.0f, 0.0f };
			l_planeBox.Extend = { FLT_MAX, 0.0f, FLT_MAX };
			// l_planeBox.Extend = { 2.0f, 0.0f, 2.0f };

			p_transformGizmo->TransformGizmoMovementGuidePlane.Box = l_planeBox;

			p_transformGizmo->TransformGizmoMovementGuidePlane.Collider.Box = &p_transformGizmo->TransformGizmoMovementGuidePlane.Box;
			p_transformGizmo->TransformGizmoMovementGuidePlane.Collider.Transform = &p_transformGizmo->TransformGizmoMovementGuidePlane.Transform;

			Vector3<float> tmp_vec3 = { 1.0f, 1.0f, 1.0f };
			TransformM::setLocalScale(&p_transformGizmo->TransformGizmoMovementGuidePlane.Transform, &tmp_vec3);
		}
	}

	void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs)
	{
		_ECS::ECSEventMessage* l_eraseEntitymessage = _ECS::ECSEventMessage_removeEntity_alloc(&p_transformGizmo->TransformGizoEntity->ComponentHeader.AttachedEntity);
		_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_eraseEntitymessage);
		*p_transformGizmo = {};
		_ECS::ECSEventQueue_processMessages(&p_ecs->EventQueue);
	};

	void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(_GameEngineEditor::EntitySelection* p_entitySelection, Transform& p_followedTransform)
	{
		Vector4<float> tmp_vec4_0;
		// In order for the transform gimo to always have the same visible size, we fix it's z clip space position.
		{
			_ECS::TransformComponent* l_transformGizmotransform = p_entitySelection->TransformGizmoV2.TransformGizoEntity;
			if (l_transformGizmotransform)
			{
				Vector3<float> l_followedWorldPosition; TransformM::getWorldPosition(&p_followedTransform, &l_followedWorldPosition);
				Quaternion<float> l_followedRotation; TransformM::getWorldRotation(&p_followedTransform, &l_followedRotation);

				Vector3<float> l_transformGizmoWorldPosition;
				{
					Matrix4x4<float> l_worldToClipMatrix = _ECS::Camera_worldToClipMatrix(p_entitySelection->EntitySelectionState.CachedStructures.ActiveCamera);
					Matrix4x4<float> l_clipToWorldMatrix; MatrixM::inv(&l_worldToClipMatrix, &l_clipToWorldMatrix);

					Vector4<float> l_selectedEntityTransformClip; MatrixM::clipSpaceMul(&l_worldToClipMatrix, &VectorM::cast(&l_followedWorldPosition, 1.0f), &l_selectedEntityTransformClip);
					l_selectedEntityTransformClip.z = 0.99f; //Fixed distance in clip space from near plane.
					l_transformGizmoWorldPosition = *VectorM::cast(MatrixM::clipSpaceMul(&l_clipToWorldMatrix, &l_selectedEntityTransformClip, &tmp_vec4_0));
				}

				TransformM::setWorldPosition(&l_transformGizmotransform->Transform, &l_transformGizmoWorldPosition);
				TransformM::setLocalRotation(&l_transformGizmotransform->Transform, &l_followedRotation);
			}
		}
	};

	bool TransformGizmo_detemineGizmoTypeSwitch(EntitySelection* p_entitySelection, SelectedGizmoType* p_out)
	{
		if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::T, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
		{
			*p_out = SelectedGizmoType::ARROW;
			return true;
		}
		else if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::R, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
		{
			*p_out = SelectedGizmoType::ROTATION;
			return true;
		}
		else if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::S, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
		{
			*p_out = SelectedGizmoType::SCALE;
			return true;
		}

		return false;
	};

	void TransformGizmo_siwtchGizmoType(EntitySelection* p_entitySelection, SelectedGizmoType p_newGizmoType)
	{
		if (p_entitySelection->EntitySelectionState.TransformGizmoSelectionState.SelectedGizmoType != p_newGizmoType)
		{
			Vector3<float> l_oldWorldPosition; TransformM::getWorldPosition(&p_entitySelection->TransformGizmoV2.TransformGizoEntity->Transform, &l_oldWorldPosition);
			p_entitySelection->EntitySelectionState.TransformGizmoSelectionState.SelectedGizmoType = p_newGizmoType;
			TransformGizmoV2_alloc(p_entitySelection, l_oldWorldPosition, p_entitySelection->ECS);
		};
	};

	_ECS::TransformComponent* TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, SegmentV2<3, float>* p_collisionRay)
	{
		_Physics::BoxCollider* l_transformArrowCollidersPtr[3];
		_Core::ArrayT<_Physics::BoxCollider*> l_transformArrowColliders = _Core::ArrayT_fromCStyleArray(l_transformArrowCollidersPtr, 3);
		l_transformArrowColliders.Size = 0;
		{
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->RightGizmo->ComponentHeader.AttachedEntity)->Boxcollider);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->UpGizmo->ComponentHeader.AttachedEntity)->Boxcollider);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->ForwardGizmo->ComponentHeader.AttachedEntity)->Boxcollider);
		}
		_Physics::RaycastHit l_hit;
		if (_Physics::RayCast_against(&l_transformArrowColliders, p_collisionRay->Begin, p_collisionRay->End, &l_hit))
		{
			return _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
		}

		return nullptr;
	}

	void TransformGizmo_setSelectedGizmo(TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedGizmo)
	{
		Vector3<float> tmp_vec;
		if (p_selectionState->SelectedGizmo)
		{
			tmp_vec = { 1.0f, 1.0f, 1.0f };
			TransformM::setLocalScale(&p_selectionState->SelectedGizmo->Transform, &tmp_vec);
		}
		if (p_selectedGizmo)
		{
			tmp_vec = { 1.2f, 1.2f, 1.2f };
			TransformM::setLocalScale(&p_selectedGizmo->Transform, &tmp_vec);
		}
		p_selectionState->SelectedGizmo = p_selectedGizmo;
	}

	void TransformGizmo_resetState(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState)
	{
		p_selectionState->GuidePlaneRotationSet = false;
		TransformGizmo_setSelectedGizmo(p_selectionState, nullptr);
	};

	bool TransformGizmoSelectionState_anyGizmoSelected(TransformGizmoSelectionState* p_transformGizmoSelectionState)
	{
		return p_transformGizmoSelectionState->SelectedGizmo;
	};
}