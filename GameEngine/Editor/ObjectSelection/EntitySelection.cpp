#include "EntitySelection.h"

#include <float.h>

#include "Editor/GameEngineEditor.h"
#include "GameEngineApplicationInterface.h"

#include "Math/Math.h"
#include "Math/Segment/Segment.h"
#include "Math/Intersection/Intersection.h"
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

namespace _GameEngineEditor
{
	void EntitySelection_onCameraSystem_enabled(EntitySelection* p_entitySelection, _ECS::CameraSystem** p_cameraSystem);

	void TransformGizmoV2_alloc(TransformGizmo* p_transformGizmo, _MathV2::Vector3<float>& p_initialWorldPosition, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface);
	void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs);
	void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(_GameEngineEditor::EntitySelection* p_entitySelection, _Math::Transform* p_followedTransform);
	TransformGizmoSelectionState TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, _Math::Segment* p_collisionRay);
	void TransformGizmo_setSelectedArrow(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedArrow);
	void TransformGizmo_setSelectedRotation(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedRotation);

	void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, _ECS::Entity* p_selectedEntity);
	void EntitySelection_moveSelectedEntity_arrowTranslation(_GameEngineEditor::EntitySelection* p_entitySelection);
	void EntitySelection_rotateSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection);

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
		p_entitySelection->CachedStructures.ActiveCamera = _ECS::CameraSystem_getCurrentActiveCamera(p_entitySelection->CachedStructures.CameraSystem);

		// Trying to detect the selected Entity
		if (!EntitSelection_isEntitySelected(p_entitySelection))
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				_Math::Segment l_ray =
					_ECS::Camera_buildWorldSpaceRay(
						p_entitySelection->CachedStructures.ActiveCamera,
						_MathV2::Vector2<float> { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y }
				);

				_Physics::RaycastHit l_hit;
				if (_Physics::RayCast(p_entitySelection->PhysicsInterface->World, l_ray.Begin, l_ray.End, &l_hit))
				{
					_ECS::TransformComponent* l_transformComponent = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
					p_entitySelection->SelectedEntity = l_transformComponent->ComponentHeader.AttachedEntity;
					TransformGizmoV2_alloc(&p_entitySelection->TransformGizmoV2, _Math::Transform_getWorldPosition(&l_transformComponent->Transform), p_entitySelection->ECS, p_entitySelection->RenderInterface);
					_ECS::ECSEventQueue_processMessages(&p_entitySelection->ECS->EventQueue);
					return; //We return to wait for the next frame where transform colliders and box will be calculated
				}
			}
		}


		if (EntitSelection_isEntitySelected(p_entitySelection))
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				_Math::Segment l_ray =
					_ECS::Camera_buildWorldSpaceRay(
						p_entitySelection->CachedStructures.ActiveCamera,
						_MathV2::Vector2<float>	{ (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y }
				);

				TransformGizmoSelectionState l_currentFrame_transformGizmoSelectionState = TransformGizmo_determinedSelectedGizmoComponent(&p_entitySelection->TransformGizmoV2, &l_ray);
				TransformGizmoSelectionState* l_transformGizmoSelectionState = &p_entitySelection->TransformGizmoSelectionState;
				{
					TransformGizmo_setSelectedArrow(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, l_currentFrame_transformGizmoSelectionState.SelectedArrow);
					TransformGizmo_setSelectedRotation(&p_entitySelection->TransformGizmoV2, &p_entitySelection->TransformGizmoSelectionState, l_currentFrame_transformGizmoSelectionState.SelectedRotation);
				}

				if (!l_currentFrame_transformGizmoSelectionState.SelectedArrow && !l_currentFrame_transformGizmoSelectionState.SelectedRotation)
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
			}

			// MOVE
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED))
			{
				if (!_MathV2::VectorM::equals(p_entitySelection->Input->InputMouse.MouseDelta, { 0.0, 0.0 }))
				{
					if (p_entitySelection->TransformGizmoSelectionState.SelectedArrow)
					{
						EntitySelection_moveSelectedEntity_arrowTranslation(p_entitySelection);
					}
					else if (p_entitySelection->TransformGizmoSelectionState.SelectedRotation)
					{
						EntitySelection_rotateSelectedEntity(p_entitySelection);
					}
				}
			}

		}



		if (EntitSelection_isEntitySelected(p_entitySelection))
		{
			_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
			TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(p_entitySelection, &l_selectedEntityTransform->Transform);
			EntitySelection_drawSelectedEntityBoundingBox(p_entitySelection, p_entitySelection->SelectedEntity);
		}
	}

	_Math::Segment entitySelection_rayCastMouseDeltaPosition_againstPlane(_GameEngineEditor::EntitySelection* p_entitySelection, _Physics::BoxCollider* p_testedCollider)
	{
		_Math::Segment l_mouseDelta_worldPosition{};
		_Math::Segment l_mouseDelta_screenPosition = _Input::InputMouse_getMouseDeltaScreenPosition(&p_entitySelection->Input->InputMouse);


		_Math::Segment l_mouseDelta_begin_ray =
			_ECS::Camera_buildWorldSpaceRay(
				p_entitySelection->CachedStructures.ActiveCamera,
				_MathV2::VectorM::cast(*(_MathV2::Vector3<float>*) & l_mouseDelta_screenPosition.Begin)
			);
		_Math::Segment l_mouseDelta_end_ray =
			_ECS::Camera_buildWorldSpaceRay(
				p_entitySelection->CachedStructures.ActiveCamera,
				_MathV2::VectorM::cast(*(_MathV2::Vector3<float>*) & l_mouseDelta_screenPosition.End)
			);

		_Physics::BoxCollider* l_raycastedPlane_ptr[1] = { p_testedCollider };
		_Core::ArrayT<_Physics::BoxCollider*> l_raycastedPlane = _Core::ArrayT_fromCStyleArray(l_raycastedPlane_ptr, 1);
		_Physics::RaycastHit l_endHit;
		if (_Physics::RayCast_against(&l_raycastedPlane, l_mouseDelta_end_ray.Begin, l_mouseDelta_end_ray.End, &l_endHit))
		{
			// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_endHit.HitPoint);
			l_mouseDelta_worldPosition.End = *(_MathV2::Vector3<float>*) & l_endHit.HitPoint;
		}
		_Physics::RaycastHit l_beginHit;
		if (_Physics::RayCast_against(&l_raycastedPlane, l_mouseDelta_begin_ray.Begin, l_mouseDelta_begin_ray.End, &l_beginHit))
		{
			// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_beginHit.HitPoint);
			l_mouseDelta_worldPosition.Begin = *(_MathV2::Vector3<float>*) & l_beginHit.HitPoint;
		}

		_Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Box,
			_Math::Transform_getLocalToWorldMatrix(&p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane.Transform), true);

		return l_mouseDelta_worldPosition;
	}

	void EntitySelection_moveSelectedEntity_arrowTranslation(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
		_ECS::TransformComponent* l_selectedArrow = p_entitySelection->TransformGizmoSelectionState.SelectedArrow;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		// We position the world space place
		{
			// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
			_Math::Transform_setWorldPosition(&l_transformGizmoPlane->Transform, _Math::Transform_getWorldPosition(&l_transformComponent->Transform));
			_Math::Transform_setLocalRotation(&l_transformGizmoPlane->Transform, _Math::Transform_getWorldRotation(&l_selectedArrow->Transform));

			// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &l_transformGizmoPlane->Box, _Math::Transform_getLocalToWorldMatrix_ref(&l_transformGizmoPlane->Transform), true);
		}

		_MathV2::Vector3<float> l_deltaPosition =
			_MathV2::VectorM::project(
				_Math::Segment_toVector(entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider)),
				_Math::Transform_getForward(&l_selectedArrow->Transform)
			);
		_Math::Transform_addToWorldPosition(&(l_transformComponent)->Transform, l_deltaPosition);
	}

	void EntitySelection_rotateSelectedEntity(_GameEngineEditor::EntitySelection* p_entitySelection)
	{
		_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
		_ECS::TransformComponent* l_selectedRotation = p_entitySelection->TransformGizmoSelectionState.SelectedRotation;
		TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoMovementGuidePlane;

		//We position the guide plane
		_MathV2::Vector3<float> l_guidePlane_worldPosition = _Math::Transform_getWorldPosition(&l_transformComponent->Transform);
		_Math::Transform_setWorldPosition(&l_transformGizmoPlane->Transform, l_guidePlane_worldPosition);
		if (l_selectedRotation == p_entitySelection->TransformGizmoV2.XRotation)
		{
			_Math::Transform_setLocalRotation(&l_transformGizmoPlane->Transform,
				_MathV2::QuaternionM::mul(
					_Math::Transform_getWorldRotation(&p_entitySelection->TransformGizmoV2.RightArrow->Transform),
					_MathV2::QuaternionM::rotateAround(_MathV2::RIGHT, M_PI * 0.5f)
				)
			);
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.YRotation)
		{
			_Math::Transform_setLocalRotation(&l_transformGizmoPlane->Transform, _Math::Transform_getWorldRotation(&p_entitySelection->TransformGizmoV2.ZRotation->Transform));
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.ZRotation)
		{
			_Math::Transform_setLocalRotation(&l_transformGizmoPlane->Transform, _Math::Transform_getWorldRotation(&p_entitySelection->TransformGizmoV2.UpArrow->Transform));
		}

		_Math::Segment l_deltaPositionDirection_worldSpace = entitySelection_rayCastMouseDeltaPosition_againstPlane(p_entitySelection, &l_transformGizmoPlane->Collider);
		_Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, l_deltaPositionDirection_worldSpace.Begin);
		_Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, l_deltaPositionDirection_worldSpace.End);

		// Perform rotation.

		_MathV2::Vector3<float> l_axis;

		if (l_selectedRotation == p_entitySelection->TransformGizmoV2.XRotation)
		{
			l_axis = _MathV2::RIGHT;
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.YRotation)
		{
			l_axis = _MathV2::UP;
		}
		else if (l_selectedRotation == p_entitySelection->TransformGizmoV2.ZRotation)
		{
			l_axis = _MathV2::FORWARD;
		}

		float l_deltaRotation =
			_MathV2::VectorM::angle_normalized(
				_MathV2::VectorM::normalize(*(_MathV2::Vector3<float>*) & _MathV2::VectorM::min(*(_MathV2::Vector3<float>*) & l_deltaPositionDirection_worldSpace.Begin, *(_MathV2::Vector3<float>*) & l_guidePlane_worldPosition)),
				_MathV2::VectorM::normalize(*(_MathV2::Vector3<float>*) & _MathV2::VectorM::min(*(_MathV2::Vector3<float>*) & l_deltaPositionDirection_worldSpace.End, *(_MathV2::Vector3<float>*) & l_guidePlane_worldPosition))
			);

		_MathV2::Quaternion<float> l_nextRotation = _MathV2::QuaternionM::mul(l_transformComponent->Transform.LocalRotation, _MathV2::QuaternionM::rotateAround(l_axis, l_deltaRotation));
		_Math::Transform_setLocalRotation(&l_transformComponent->Transform, l_nextRotation);
	};

	void EntitySelection_drawSelectedEntityBoundingBox(EntitySelection* p_entitySelection, _ECS::Entity* p_selectedEntity)
	{
		_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_selectedEntity);
		_ECS::MeshRendererBound* l_meshRendererBound = _ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_selectedEntity);
		_Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &(l_meshRendererBound)->BoundingBox,
			_Math::Transform_getLocalToWorldMatrix(&(l_selectedEntityTransform)->Transform), true, _MathV2::Vector3<float> { 1.0f, 1.0f, 1.0f });
	}

	_ECS::TransformComponent* transformGizmoV2_allocArrow(_ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface, const _MathV2::Vector4<float>* p_color)
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
			l_transformInitInfo.LocalRotation = _MathV2::Quaternionf_Identity;
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
				{_Render::MATERIALINSTANCE_COLOR, (_MathV2::Vector4<float>*)p_color}
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

	_ECS::TransformComponent* transformGizmoV2_allocRotation(_ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface, const _MathV2::Vector4<float>* p_color)
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
			l_transformInitInfo.LocalRotation = _MathV2::Quaternionf_Identity;
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
				{_Render::MATERIALINSTANCE_COLOR, (_MathV2::Vector4<float>*)p_color}
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

	void TransformGizmoV2_alloc(TransformGizmo* p_transformGizmo, _MathV2::Vector3<float>& p_initialWorldPosition, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface)
	{
		_ECS::Entity* l_transformGizmo = _ECS::Entity_alloc();
		{
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_transformGizmo);
			_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			p_transformGizmo->TransformGizoEntity = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = p_initialWorldPosition;
			l_transformInitInfo.LocalRotation = _MathV2::Quaternionf_Identity;
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
				_MathV2::Vector4<float> l_forwardColor = { 0.0f, 0.0f, 1.0f, 1.0f };
				p_transformGizmo->ForwardArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &_Color::BLUE);
			}

			_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardArrow->Transform);
			_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightArrow->Transform);
			_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpArrow->Transform);

			_Math::Transform_setLocalRotation(&p_transformGizmo->RightArrow->Transform, _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, M_PI * 0.5f, 0.0f}));
			_Math::Transform_setLocalRotation(&p_transformGizmo->UpArrow->Transform, _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{-M_PI * 0.5f, 0.0f, 0.0f}));
			_Math::Transform_setLocalRotation(&p_transformGizmo->ForwardArrow->Transform, _MathV2::Quaternionf_Identity);
		}

		// Rotation gizmo
		{
			p_transformGizmo->XRotation = transformGizmoV2_allocRotation(p_ecs, p_renderInterface, &_Color::RED);
			p_transformGizmo->YRotation = transformGizmoV2_allocRotation(p_ecs, p_renderInterface, &_Color::GREEN);
			p_transformGizmo->ZRotation = transformGizmoV2_allocRotation(p_ecs, p_renderInterface, &_Color::BLUE);

			_Math::Transform_setLocalRotation(&p_transformGizmo->ZRotation->Transform, _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, M_PI * 0.5f, 0.0f}));
			_Math::Transform_setLocalRotation(&p_transformGizmo->YRotation->Transform, _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, 0.0f, -M_PI * 0.5f }));
			_Math::Transform_setLocalRotation(&p_transformGizmo->XRotation->Transform, _MathV2::Quaternionf_Identity);

			_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->XRotation->Transform);
			_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->YRotation->Transform);
			_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ZRotation->Transform);
		}

		//Plane instance
		{
			_Math::Box l_planeBox;
			l_planeBox.Center = { 0.0f, 0.0f, 0.0f };
			// l_planeBox.Extend = { FLT_MAX, 0.0f, FLT_MAX };
			l_planeBox.Extend = { 2.0f, 0.0f, 2.0f };

			p_transformGizmo->TransformGizmoMovementGuidePlane.Box = l_planeBox;

			p_transformGizmo->TransformGizmoMovementGuidePlane.Collider.Box = &p_transformGizmo->TransformGizmoMovementGuidePlane.Box;
			p_transformGizmo->TransformGizmoMovementGuidePlane.Collider.Transform = &p_transformGizmo->TransformGizmoMovementGuidePlane.Transform;

			_Math::Transform_setLocalScale(&p_transformGizmo->TransformGizmoMovementGuidePlane.Transform, _MathV2::Vector3<float>{ 1.0f, 1.0f, 1.0f });
		}
	}

	void TransformGizmoV2_free(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs)
	{
		_ECS::ECSEventMessage* l_eraseEntitymessage = _ECS::ECSEventMessage_removeEntity_alloc(&p_transformGizmo->TransformGizoEntity->ComponentHeader.AttachedEntity);
		_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_eraseEntitymessage);

		*p_transformGizmo = {};
	};

	void TransformGizmo_followTransform_byKeepingAfixedDistanceFromCamera(_GameEngineEditor::EntitySelection* p_entitySelection, _Math::Transform* p_followedTransform)
	{
		// In order for the transform gimo to always have the same visible size, we fix it's z clip space position.
		{
			_ECS::TransformComponent* l_transformGizmotransform = p_entitySelection->TransformGizmoV2.TransformGizoEntity;
			if (l_transformGizmotransform)
			{
				_MathV2::Vector3<float> l_followedWorldPosition = _Math::Transform_getWorldPosition(p_followedTransform);
				_MathV2::Quaternion<float> l_followedRotation = _Math::Transform_getWorldRotation(p_followedTransform);

				_MathV2::Vector3<float> l_transformGizmoWorldPosition;
				{
					_MathV2::Matrix4x4<float> l_worldToClipMatrix = _ECS::Camera_worldToClipMatrix(p_entitySelection->CachedStructures.ActiveCamera);
					_MathV2::Matrix4x4<float> l_clipToWorldMatrix = _MathV2::MatrixM::inv(l_worldToClipMatrix);

					_MathV2::Vector4<float> l_selectedEntityTransformClip = _MathV2::MatrixM::clipSpaceMul(l_worldToClipMatrix, _MathV2::VectorM::cast(l_followedWorldPosition, 1.0f));
					l_selectedEntityTransformClip.z = 0.99f; //Fixed distance in clip space from near plane.
					l_transformGizmoWorldPosition = _MathV2::VectorM::cast(_MathV2::MatrixM::clipSpaceMul(l_clipToWorldMatrix, l_selectedEntityTransformClip));
				}

				_Math::Transform_setWorldPosition(&l_transformGizmotransform->Transform, *(_MathV2::Vector3<float>*) & l_transformGizmoWorldPosition);
				_Math::Transform_setLocalRotation(&l_transformGizmotransform->Transform, l_followedRotation);
			}
		}
	};

	TransformGizmoSelectionState TransformGizmo_determinedSelectedGizmoComponent(TransformGizmo* p_transformGizmo, _Math::Segment* p_collisionRay)
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

		}

		return l_gizmoSelectionState;
	}

	void TransformGizmo_setSelectedArrow(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedArrow)
	{
		if (p_selectionState->SelectedArrow)
		{
			_Math::Transform_setLocalScale(&p_selectionState->SelectedArrow->Transform, _MathV2::Vector3<float>{ 1.0f,1.0f,1.0f });
		}
		if (p_selectedArrow)
		{
			_Math::Transform_setLocalScale(&p_selectedArrow->Transform, _MathV2::Vector3<float>{ 1.2f,1.2f,1.2f });
		}
		p_selectionState->SelectedArrow = p_selectedArrow;
	}

	void TransformGizmo_setSelectedRotation(TransformGizmo* p_transformGizmo, TransformGizmoSelectionState* p_selectionState, _ECS::TransformComponent* p_selectedRotation)
	{
		if (p_selectionState->SelectedRotation)
		{
			_Math::Transform_setLocalScale(&p_selectionState->SelectedRotation->Transform, _MathV2::Vector3<float>{ 1.0f,1.0f,1.0f });
		}
		if (p_selectedRotation)
		{
			_Math::Transform_setLocalScale(&p_selectedRotation->Transform, _MathV2::Vector3<float>{ 1.2f,1.2f,1.2f });
		}
		p_selectionState->SelectedRotation = p_selectedRotation;
	}
}