#include "EntitySelection.h"

#include <float.h>

#include "Editor/GameEngineEditor.h"
#include "GameEngineApplicationInterface.h"

#include "v2/Math.h"
#include "v2/Segment/SegmentMath.hpp"
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

namespace _GameEngineEditor
{
	void EntitySelection_onCameraSystem_enabled(EntitySelection* p_entitySelection, _ECS::CameraSystem** p_cameraSystem);

	void TransformGizmoV2_alloc(TransformGizmo* p_transformGizmo, Vector3<float>& p_initialWorldPosition, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface);
	void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs);
	void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(_GameEngineEditor::EntitySelection* p_entitySelection, Transform& p_followedTransform);
	TransformGizmoSelectionState TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, Segment* p_collisionRay);
	void TransformGizmo_setSelectedArrow(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedArrow);
	void TransformGizmo_setSelectedRotation(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedRotation);
	void TransformGizmo_setSelectedScale(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedScale);

	void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, _ECS::Entity* p_selectedEntity);
	void EntitySelection_moveSelectedEntity_arrowTranslation(_GameEngineEditor::EntitySelection* p_entitySelection);
	void EntitySelection_rotateSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection);
	void EntitySelection_scaleSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection);

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor)
	{
		p_entitySelection->ECS = p_gameEngineEditor->GameEngineApplicationInterface->ECS;
		p_entitySelection->Input = p_gameEngineEditor->GameEngineApplicationInterface->Input;
		p_entitySelection->RenderInterface = p_gameEngineEditor->GameEngineApplicationInterface->RenderInterface;
		p_entitySelection->PhysicsInterface = p_gameEngineEditor->GameEngineApplicationInterface->PhysicsInterface;
		p_entitySelection->SelectedEntity = nullptr;

		_Core::CallbackT < EntitySelection, _ECS::CameraSystem* > l_onCameraSystemEnabled = { EntitySelection_onCameraSystem_enabled , p_entitySelection };
		_ECS::SystemEvents_registerOnSystemAdded(&p_gameEngineEditor->GameEngineApplicationInterface->ECS->SystemEvents, _ECS::CameraSystemKey, (_Core::CallbackT<void, _ECS::SystemHeader*>*) & l_onCameraSystemEnabled);
	};

	inline bool EntitySelection_isCachedStructureInitialized(EntitySelection* p_entitySelection)
	{
		return p_entitySelection->CachedStructures.ActiveCamera;
	}

	inline bool EntitSelection_isEntitySelected(EntitySelection* p_entitySelection)
	{
		return p_entitySelection->SelectedEntity;
	}

	void EntitySelection_onCameraSystem_enabled(EntitySelection* p_entitySelection, _ECS::CameraSystem** p_cameraSystem)
	{
		p_entitySelection->CachedStructures.CameraSystem = *p_cameraSystem;
	};

	void EntitySelection_update(EntitySelection* p_entitySelection)
	{
		// Set the active camera
		_ECS::Camera* l_currentCamera = _ECS::CameraSystem_getCurrentActiveCamera(p_entitySelection->CachedStructures.CameraSystem);
		if (l_currentCamera != p_entitySelection->CachedStructures.ActiveCamera)
		{
			p_entitySelection->CachedStructures.ActiveCamera = _ECS::CameraSystem_getCurrentActiveCamera(p_entitySelection->CachedStructures.CameraSystem);
			p_entitySelection->CachedStructures.ActiveCameraTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->CachedStructures.ActiveCamera->ComponentHeader.AttachedEntity);
		}

		// Trying to detect the selected Entity
		if (!EntitSelection_isEntitySelected(p_entitySelection))
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				Segment l_ray =
					_ECS::Camera_buildWorldSpaceRay(
						p_entitySelection->CachedStructures.ActiveCamera,
						Vector2<float> { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y }
				);

				_Physics::RaycastHit l_hit;
				if (_Physics::RayCast(p_entitySelection->PhysicsInterface->World, l_ray.Begin, l_ray.End, &l_hit))
				{
					Vector3<float> tmp_vec3;
					_ECS::TransformComponent* l_transformComponent = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
					p_entitySelection->SelectedEntity = l_transformComponent->ComponentHeader.AttachedEntity;
					TransformGizmoV2_alloc(&p_entitySelection->TransformGizmoV2, *TransformM::getWorldPosition(&l_transformComponent->Transform, &tmp_vec3), p_entitySelection->ECS, p_entitySelection->RenderInterface);
					_ECS::ECSEventQueue_processMessages(&p_entitySelection->ECS->EventQueue);
					return; //We return to wait for the next frame where transform colliders and box will be calculated
				}
			}
		}


		if (EntitSelection_isEntitySelected(p_entitySelection))
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				Segment l_ray =
					_ECS::Camera_buildWorldSpaceRay(
						p_entitySelection->CachedStructures.ActiveCamera,
						Vector2<float>	{ (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y }
				);

				TransformGizmoSelectionState l_currentFrame_transformGizmoSelectionState = TransformGizmo_determinedSelectedGizmoComponent(&p_entitySelection->TransformGizmoV2, &l_ray);
				TransformGizmoSelectionState* l_transformGizmoSelectionState = &p_entitySelection->TransformGizmoSelectionState;
				{
					TransformGizmo_setSelectedArrow(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, l_currentFrame_transformGizmoSelectionState.SelectedArrow);
					TransformGizmo_setSelectedRotation(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, l_currentFrame_transformGizmoSelectionState.SelectedRotation);
					TransformGizmo_setSelectedScale(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, l_currentFrame_transformGizmoSelectionState.SelectedScale);
				}

				if (!l_currentFrame_transformGizmoSelectionState.SelectedArrow && !l_currentFrame_transformGizmoSelectionState.SelectedRotation && !l_currentFrame_transformGizmoSelectionState.SelectedScale)
				{
					TransformGizmoV2_free(&p_entitySelection->TransformGizmoV2, p_entitySelection->ECS);
					p_entitySelection->SelectedEntity = nullptr;
					_ECS::ECSEventQueue_processMessages(&p_entitySelection->ECS->EventQueue);
				}

			}
			else if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::RELEASED_THIS_FRAME))
			{
				TransformGizmo_setSelectedArrow(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, nullptr);
				TransformGizmo_setSelectedRotation(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, nullptr);
				TransformGizmo_setSelectedScale(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, nullptr);
			}

			// MOVE
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED))
			{
				Vector2<double> tmp_vec2 = { 0.0, 0.0 };
				if (!VectorM::equals(&p_entitySelection->Input->InputMouse.MouseDelta, &tmp_vec2))
				{
					if (p_entitySelection->TransformGizmoSelectionState.SelectedArrow)
					{
						EntitySelection_moveSelectedEntity_arrowTranslation(p_entitySelection);
					}
					else if (p_entitySelection->TransformGizmoSelectionState.SelectedRotation)
					{
						EntitySelection_rotateSelectedEntity(p_entitySelection);
					}
					else if (p_entitySelection->TransformGizmoSelectionState.SelectedScale)
					{
						EntitySelection_scaleSelectedEntity(p_entitySelection);
					}
				}
			}

		}



		if (EntitSelection_isEntitySelected(p_entitySelection))
		{
			_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
			TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(p_entitySelection, l_selectedEntityTransform->Transform);
			EntitySelection_drawSelectedEntityBoundingBox(p_entitySelection, p_entitySelection->SelectedEntity);
		}
	}

	Segment entitySelection_rayCastMouseDeltaPosition_againstPlane(_GameEngineEditor::EntitySelection* p_entitySelection, _Physics::BoxCollider* p_testedCollider)
	{
		Segment l_mouseDelta_worldPosition{};
		Segment l_mouseDelta_screenPosition = _Input::InputMouse_getMouseDeltaScreenPosition(&p_entitySelection->Input->InputMouse);


		Segment l_mouseDelta_begin_ray =
			_ECS::Camera_buildWorldSpaceRay(
				p_entitySelection->CachedStructures.ActiveCamera,
				*VectorM::cast(&l_mouseDelta_screenPosition.Begin)
			);
		Segment l_mouseDelta_end_ray =
			_ECS::Camera_buildWorldSpaceRay(
				p_entitySelection->CachedStructures.ActiveCamera,
				*VectorM::cast(&l_mouseDelta_screenPosition.End)
			);

		_Physics::BoxCollider* l_raycastedPlane_ptr[1] = { p_testedCollider };
		_Core::ArrayT<_Physics::BoxCollider*> l_raycastedPlane = _Core::ArrayT_fromCStyleArray(l_raycastedPlane_ptr, 1);
		_Physics::RaycastHit l_endHit;
		if (_Physics::RayCast_against(&l_raycastedPlane, l_mouseDelta_end_ray.Begin, l_mouseDelta_end_ray.End, &l_endHit))
		{
			// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_endHit.HitPoint);
			l_mouseDelta_worldPosition.End = *(Vector3<float>*) & l_endHit.HitPoint;
		}
		_Physics::RaycastHit l_beginHit;
		if (_Physics::RayCast_against(&l_raycastedPlane, l_mouseDelta_begin_ray.Begin, l_mouseDelta_begin_ray.End, &l_beginHit))
		{
			// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_beginHit.HitPoint);
			l_mouseDelta_worldPosition.Begin = *(Vector3<float>*) & l_beginHit.HitPoint;
		}

		Matrix4x4<float> tmp_mat4_0;
		_Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Box,
			TransformM::getLocalToWorldMatrix(&p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Transform, &tmp_mat4_0), true);

		return l_mouseDelta_worldPosition;
	}

	void EntitySelection_moveSelectedEntity_arrowTranslation(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		Vector3<float> tmp_vec3_1, tmp_vec3_0; Quaternion<float> tmp_quat_1;

		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
		_ECS::TransformComponent* l_selectedArrow = p_entitySelection->TransformGizmoSelectionState.SelectedArrow;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		// We position the world space place
		{
			// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
			TransformM::setWorldPosition(&l_transformGizmoPlane->Transform, TransformM::getWorldPosition(&l_transformComponent->Transform, &tmp_vec3_1));
			TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&l_selectedArrow->Transform, &tmp_quat_1));

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
		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
		_ECS::TransformComponent* l_selectedRotation = p_entitySelection->TransformGizmoSelectionState.SelectedRotation;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		//We position the guide plane
		Vector3<float> l_guidePlane_worldPosition; TransformM::getWorldPosition(&l_transformComponent->Transform, &l_guidePlane_worldPosition);
		TransformM::setWorldPosition(&l_transformGizmoPlane->Transform, &l_guidePlane_worldPosition);
		if (l_selectedRotation == p_entitySelection->TransformGizmoV2.XRotation)
		{
			Quaternion<float> tmp_quat_0, tmp_quat_1, tmp_quat_2;
			TransformM::setLocalRotation(&l_transformGizmoPlane->Transform,
				QuaternionM::mul(
					TransformM::getWorldRotation(&p_entitySelection->TransformGizmoV2.RightArrow->Transform, &tmp_quat_0),
					QuaternionM::rotateAround(&RIGHT, M_PI * 0.5f, &tmp_quat_1),
					&tmp_quat_2
				)
			);
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.YRotation)
		{
			Quaternion<float> tmp_quat;
			TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&p_entitySelection->TransformGizmoV2.ZRotation->Transform, &tmp_quat));
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.ZRotation)
		{
			Quaternion<float> tmp_quat;
			TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&p_entitySelection->TransformGizmoV2.UpArrow->Transform, &tmp_quat));
		}

		Segment l_deltaPositionDirection_worldSpace = entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider);
		_Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_deltaPositionDirection_worldSpace.Begin);
		_Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_deltaPositionDirection_worldSpace.End);

		// Perform rotation.

		Vector3<float> l_axis_worldSpace;
		{
			Vector3<float> l_axis_localSpace;
			if (l_selectedRotation == p_entitySelection->TransformGizmoV2.XRotation)
			{
				l_axis_localSpace = RIGHT;
			}
			else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.YRotation)
			{
				l_axis_localSpace = UP;
			}
			else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.ZRotation)
			{
				l_axis_localSpace = FORWARD;
			}

			Matrix4x4<float> tmp_mat4_0; Vector4<float> tmp_vec4_0;
			VectorM::normalize(
				VectorM::min(
					VectorM::cast(MatrixM::mul(TransformM::getLocalToWorldMatrix(&l_transformComponent->Transform, &tmp_mat4_0), &VectorM::cast(&l_axis_localSpace, 1.0f), &tmp_vec4_0)),
					&l_guidePlane_worldPosition,
					&l_axis_worldSpace
				),
				&l_axis_worldSpace
			);
		}

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
			VectorM::normalize(VectorM::min(TransformM::getWorldPosition(&p_entitySelection->CachedStructures.ActiveCameraTransform->Transform, &tmp_vec3_0), &l_guidePlane_worldPosition, &l_angleReferenceAxis), &l_angleReferenceAxis);
			l_deltaRotation *= VectorM::angleSign(&l_rotationBegin, &l_rotationEnd, &l_angleReferenceAxis);

			// If the rotation axis is not facing the camera, we invert the sign.
			if (VectorM::dot(&l_angleReferenceAxis, &l_axis_worldSpace) <= 0.000f)
			{
				l_deltaRotation *= -1.0f;
			}
		}

		Quaternion<float> tmp_quat_0, tmp_quat_1;
		Quaternion<float> l_nextRotation;
		QuaternionM::mul(
			QuaternionM::rotateAround(&l_axis_worldSpace, l_deltaRotation, &tmp_quat_0),
			TransformM::getWorldRotation(&l_transformComponent->Transform, &tmp_quat_1),
			&l_nextRotation
		);

		TransformM::setWorldRotation(&l_transformComponent->Transform, &l_nextRotation);
	};

	void EntitySelection_scaleSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		Vector3<float> tmp_vec3_1, tmp_vec3_0; Quaternion<float> tmp_quat_1;

		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
		_ECS::TransformComponent* l_selectedScale = p_entitySelection->TransformGizmoSelectionState.SelectedScale;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		// We position the world space place
		{
			// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
			TransformM::setWorldPosition(&l_transformGizmoPlane->Transform, TransformM::getWorldPosition(&l_transformComponent->Transform, &tmp_vec3_1));
			TransformM::setLocalRotation(&l_transformGizmoPlane->Transform, TransformM::getWorldRotation(&l_selectedScale->Transform, &tmp_quat_1));

			// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &l_transformGizmoPlane->Box, TransformM::getLocalToWorldMatrix_ref(&l_transformGizmoPlane->Transform), true);
		}

		Vector3<float> l_deltaScale3D;
		VectorM::project(
			SegmentM::toVector(&entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider), &tmp_vec3_1),
			TransformM::getForward(&l_selectedScale->Transform, &tmp_vec3_0),
			&l_deltaScale3D
		);

		TransformM::setLocalScale(&l_transformComponent->Transform, VectorM::add(&l_transformComponent->Transform.LocalScale, &l_deltaScale3D, &tmp_vec3_1));
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

	void TransformGizmoV2_alloc(TransformGizmo* p_transformGizmo, Vector3<float>& p_initialWorldPosition, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface)
	{
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

		//Arrow transform gizmo
		{
			{
				p_transformGizmo->RightArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &_Color::RED);
			}
			{
				p_transformGizmo->UpArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &_Color::GREEN);
			}
			{
				p_transformGizmo->ForwardArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &_Color::BLUE);
			}

			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardArrow->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightArrow->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpArrow->Transform);

			tmp_vec3_0 = { 0.0f, M_PI * 0.5f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->RightArrow->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			tmp_vec3_0 = { -M_PI * 0.5f, 0.0f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->UpArrow->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			TransformM::setLocalRotation(&p_transformGizmo->ForwardArrow->Transform, &Quaternionf_Identity);
		}

		// Rotation gizmo
		{
			p_transformGizmo->XRotation = transformGizmoV2_allocRotation(p_ecs, p_renderInterface, &_Color::RED);
			p_transformGizmo->YRotation = transformGizmoV2_allocRotation(p_ecs, p_renderInterface, &_Color::GREEN);
			p_transformGizmo->ZRotation = transformGizmoV2_allocRotation(p_ecs, p_renderInterface, &_Color::BLUE);

			tmp_vec3_0 = { 0.0f, M_PI * 0.5f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->ZRotation->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			tmp_vec3_0 = { 0.0f, 0.0f, -M_PI * 0.5f };
			TransformM::setLocalRotation(&p_transformGizmo->YRotation->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			TransformM::setLocalRotation(&p_transformGizmo->XRotation->Transform, &Quaternionf_Identity);

			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->XRotation->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->YRotation->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ZRotation->Transform);
		}

		// Scale gizmo
		{
			{
				p_transformGizmo->RightScale = transformGizmoV2_allocScale(p_ecs, p_renderInterface, &_Color::RED);
			}
			{
				p_transformGizmo->UpScale = transformGizmoV2_allocScale(p_ecs, p_renderInterface, &_Color::GREEN);
			}
			{
				p_transformGizmo->ForwardScale = transformGizmoV2_allocScale(p_ecs, p_renderInterface, &_Color::BLUE);
			}

			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightScale->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpScale->Transform);
			TransformM::addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardScale->Transform);

			tmp_vec3_0 = { 0.0f, M_PI * 0.5f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->RightScale->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			tmp_vec3_0 = { -M_PI * 0.5f, 0.0f, 0.0f };
			TransformM::setLocalRotation(&p_transformGizmo->UpScale->Transform, QuaternionM::fromEulerAngle(&tmp_vec3_0, &tmp_quat_0));
			TransformM::setLocalRotation(&p_transformGizmo->ForwardScale->Transform, &Quaternionf_Identity);
		}

		//Plane instance
		{
			Box l_planeBox;
			l_planeBox.Center = { 0.0f, 0.0f, 0.0f };
			// l_planeBox.Extend = { FLT_MAX, 0.0f, FLT_MAX };
			l_planeBox.Extend = { 2.0f, 0.0f, 2.0f };

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
					Matrix4x4<float> l_worldToClipMatrix = _ECS::Camera_worldToClipMatrix(p_entitySelection->CachedStructures.ActiveCamera);
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

	TransformGizmoSelectionState TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, Segment* p_collisionRay)
	{
		TransformGizmoSelectionState l_gizmoSelectionState{};

		_Physics::BoxCollider* l_transformArrowCollidersPtr[3];
		_Core::ArrayT<_Physics::BoxCollider*> l_transformArrowColliders = _Core::ArrayT_fromCStyleArray(l_transformArrowCollidersPtr, 3);
		l_transformArrowColliders.Size = 0;
		{
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->RightArrow->ComponentHeader.AttachedEntity)->Boxcollider);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->UpArrow->ComponentHeader.AttachedEntity)->Boxcollider);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->ForwardArrow->ComponentHeader.AttachedEntity)->Boxcollider);
		}
		_Physics::RaycastHit l_hit;
		if (_Physics::RayCast_against(&l_transformArrowColliders, p_collisionRay->Begin, p_collisionRay->End, &l_hit))
		{
			l_gizmoSelectionState.SelectedArrow = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
		}
		else
		{
			// We try rotation
			_Core::ArrayT_clear(&l_transformArrowColliders);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->XRotation->ComponentHeader.AttachedEntity)->Boxcollider);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->YRotation->ComponentHeader.AttachedEntity)->Boxcollider);
			_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->ZRotation->ComponentHeader.AttachedEntity)->Boxcollider);
			if (_Physics::RayCast_against(&l_transformArrowColliders, p_collisionRay->Begin, p_collisionRay->End, &l_hit))
			{
				l_gizmoSelectionState.SelectedRotation = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
			}
			else
			{
				// We try scale
				_Core::ArrayT_clear(&l_transformArrowColliders);
				_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->RightScale->ComponentHeader.AttachedEntity)->Boxcollider);
				_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->UpScale->ComponentHeader.AttachedEntity)->Boxcollider);
				_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_transformGizmo->ForwardScale->ComponentHeader.AttachedEntity)->Boxcollider);
				if (_Physics::RayCast_against(&l_transformArrowColliders, p_collisionRay->Begin, p_collisionRay->End, &l_hit))
				{
					l_gizmoSelectionState.SelectedScale = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
				}
			}
		}

		return l_gizmoSelectionState;
	}

	void TransformGizmo_setSelectedArrow(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedArrow)
	{
		Vector3<float> tmp_vec;
		if (p_selectionState->SelectedArrow)
		{
			tmp_vec = { 1.0f, 1.0f, 1.0f };
			TransformM::setLocalScale(&p_selectionState->SelectedArrow->Transform, &tmp_vec);
		}
		if (p_selectedArrow)
		{
			tmp_vec = { 1.2f, 1.2f, 1.2f };
			TransformM::setLocalScale(&p_selectedArrow->Transform, &tmp_vec);
		}
		p_selectionState->SelectedArrow = p_selectedArrow;
	}

	void TransformGizmo_setSelectedRotation(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedRotation)
	{
		Vector3<float> tmp_vec;
		if (p_selectionState->SelectedRotation)
		{
			tmp_vec = { 1.0f, 1.0f, 1.0f };
			TransformM::setLocalScale(&p_selectionState->SelectedRotation->Transform, &tmp_vec);
		}
		if (p_selectedRotation)
		{
			tmp_vec = { 1.2f, 1.2f, 1.2f };
			TransformM::setLocalScale(&p_selectedRotation->Transform, &tmp_vec);
		}
		p_selectionState->SelectedRotation = p_selectedRotation;
	}

	void TransformGizmo_setSelectedScale(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedScale)
	{
		Vector3<float> tmp_vec;
		if (p_selectionState->SelectedScale)
		{
			tmp_vec = { 1.0f, 1.0f, 1.0f };
			TransformM::setLocalScale(&p_selectionState->SelectedScale->Transform, &tmp_vec);
		}
		if (p_selectedScale)
		{
			tmp_vec = { 1.2f, 1.2f, 1.2f };
			TransformM::setLocalScale(&p_selectedScale->Transform, &tmp_vec);
		}
		p_selectionState->SelectedScale = p_selectedScale;
	}
}