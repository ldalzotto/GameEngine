#include "EntitySelection.h"

#include <float.h>

#include "Editor/GameEngineEditor.h"
#include "GameEngineApplicationInterface.h"

#include "Math/Math.h"
#include "Math/Segment/Segment.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Quaternion/QuaternionMath.h"

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
	void TransformGizmoV2_alloc(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface);

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor)
	{
		p_entitySelection->ECS = p_gameEngineEditor->GameEngineApplicationInterface->ECS;
		p_entitySelection->Input = p_gameEngineEditor->GameEngineApplicationInterface->Input;
		p_entitySelection->RenderInterface = p_gameEngineEditor->GameEngineApplicationInterface->RenderInterface;
		p_entitySelection->PhysicsInterface = p_gameEngineEditor->GameEngineApplicationInterface->PhysicsInterface;
		p_entitySelection->SelectedEntity = nullptr;
	};

	void EntitySelection_update(EntitySelection* p_entitySelection)
	{
		_ECS::CameraSystem* l_cameraSystem = (_ECS::CameraSystem*)_ECS::SystemContainerV2_getSystem(&p_entitySelection->ECS->SystemContainerV2, &_ECS::CameraSystemKey);
		_ECS::Camera* l_activeCamera = _ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem);
		if (!p_entitySelection->SelectedEntity)
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				_Math::Vector2f l_screenPoint = { p_entitySelection->Input->InputMouse.ScreenPosition.x, p_entitySelection->Input->InputMouse.ScreenPosition.y };
				_Math::Segment l_ray;

				_ECS::Camera_buildWorldSpaceRay(l_activeCamera, &l_screenPoint, &l_ray);

				_Physics::RaycastHit l_hit;
				if (_Physics::RayCast(p_entitySelection->PhysicsInterface->World, &l_ray.Begin, &l_ray.End, &l_hit))
				{
					_ECS::TransformComponent* l_transformComponent = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
					p_entitySelection->SelectedEntity = l_transformComponent->ComponentHeader.AttachedEntity;
				}
			}
		}

		if (p_entitySelection->SelectedEntity)
		{
			if (p_entitySelection->TransformGizmoV2.TransformGizoEntity == nullptr)
			{
				TransformGizmoV2_alloc(&p_entitySelection->TransformGizmoV2, p_entitySelection->ECS, p_entitySelection->RenderInterface);
			}
			else
			{

				if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
				{
					_Physics::BoxCollider* l_transformArrowCollidersPtr[3];
					_Core::ArrayT<_Physics::BoxCollider*> l_transformArrowColliders = _Core::ArrayT_fromCStyleArray(l_transformArrowCollidersPtr, 3);
					l_transformArrowColliders.Size = 0;
					{
						_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entitySelection->TransformGizmoV2.RightArrow->ComponentHeader.AttachedEntity)->Boxcollider);
						_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entitySelection->TransformGizmoV2.UpArrow->ComponentHeader.AttachedEntity)->Boxcollider);
						_Core::ArrayT_pushBack(&l_transformArrowColliders, &_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entitySelection->TransformGizmoV2.ForwardArrow->ComponentHeader.AttachedEntity)->Boxcollider);
					}
					_Math::Vector2f l_screenPoint = { p_entitySelection->Input->InputMouse.ScreenPosition.x, p_entitySelection->Input->InputMouse.ScreenPosition.y };
					_Math::Segment l_ray;
					_ECS::Camera_buildWorldSpaceRay(l_activeCamera, &l_screenPoint, &l_ray);
					_Physics::RaycastHit l_hit;
					if (_Physics::RayCast_against(&l_transformArrowColliders, &l_ray.Begin, &l_ray.End, &l_hit))
					{
						// The user has clicked an arrow of the transform gizmo
						p_entitySelection->SelectedTransformArrow = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
					}
					else
					{
						p_entitySelection->SelectedTransformArrow = nullptr;
					}
				}

				if (p_entitySelection->SelectedTransformArrow)
				{
					// MOVE
					if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED))
					{
						_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
						_ECS::TransformComponent* l_selectedArrow = p_entitySelection->SelectedTransformArrow;
						TransformGizmoPlane* l_transformGizmoPlane = &p_entitySelection->TransformGizmoV2.TransformGizmoSelectedArrayPlane;

						// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &l_transformGizmoPlane->Box, _Math::Transform_getLocalToWorldMatrix_ref(&l_transformGizmoPlane->Transform), true);

						_Math::Vector2d l_zero = { 0.0f, 0.0f };
						if (!_Math::Vector2d_equals(&p_entitySelection->Input->InputMouse.MouseDelta, &l_zero))
						{

							// We position the world space place
							{
								// /!\ We don't take the selectedArrow transform because it's position is not in world space (always positioned to have the same size).
								_Math::Vector3f l_worldPosition = _Math::Transform_getWorldPosition(&l_transformComponent->Transform);
								_Math::Quaternionf l_worldRotation = _Math::Transform_getWorldRotation(&l_selectedArrow->Transform);
								_Math::Transform_setLocalPosition(&l_transformGizmoPlane->Transform, l_worldPosition);
								_Math::Transform_setLocalRotation(&l_transformGizmoPlane->Transform, l_worldRotation);

								// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &l_transformGizmoPlane->Box, _Math::Transform_getLocalToWorldMatrix_ref(&l_transformGizmoPlane->Transform), true);
							}
							_Math::Vector3f l_deltaPositionDirection_worldSpace = _Math::Transform_getForward(&p_entitySelection->SelectedTransformArrow->Transform);

							_Math::Vector3f l_deltaPosition{};
							{

								_Math::Vector2f l_mouseDelta_begin = { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x - ((float)p_entitySelection->Input->InputMouse.MouseDelta.x), (float)p_entitySelection->Input->InputMouse.ScreenPosition.y - ((float)p_entitySelection->Input->InputMouse.MouseDelta.y) };
								_Math::Vector2f l_mouseDelta_end = { (float)p_entitySelection->Input->InputMouse.ScreenPosition.x, (float)p_entitySelection->Input->InputMouse.ScreenPosition.y };

								_Math::Vector3f l_mouseDelta_begin_worldSpace;
								_Math::Vector3f l_mouseDelta_end_worldSpace;

								_Math::Segment l_mouseDelta_begin_ray;
								_ECS::Camera_buildWorldSpaceRay(l_activeCamera, &l_mouseDelta_begin, &l_mouseDelta_begin_ray);
								_Math::Segment l_mouseDelta_end_ray;
								_ECS::Camera_buildWorldSpaceRay(l_activeCamera, &l_mouseDelta_end, &l_mouseDelta_end_ray);

								_Physics::BoxCollider* l_raycastedPlane_ptr[1] = { &l_transformGizmoPlane->Collider };
								_Core::ArrayT<_Physics::BoxCollider*> l_raycastedPlane = _Core::ArrayT_fromCStyleArray(l_raycastedPlane_ptr, 1);
								_Physics::RaycastHit l_endHit;
								if (_Physics::RayCast_against(&l_raycastedPlane, &l_mouseDelta_end_ray.Begin, &l_mouseDelta_end_ray.End, &l_endHit))
								{
									// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_endHit.HitPoint);
									l_mouseDelta_end_worldSpace = l_endHit.HitPoint;
								}
								_Physics::RaycastHit l_beginHit;
								if (_Physics::RayCast_against(&l_raycastedPlane, &l_mouseDelta_begin_ray.Begin, &l_mouseDelta_begin_ray.End, &l_beginHit))
								{
									// _Render::Gizmo_drawPoint(p_entitySelection->RenderInterface->Gizmo, &l_beginHit.HitPoint);
									l_mouseDelta_begin_worldSpace = l_beginHit.HitPoint;
								}

								_Math::Vector3f_min(&l_mouseDelta_end_worldSpace, &l_mouseDelta_begin_worldSpace, &l_deltaPosition);

								// We project deltaposition on the translation direction

								_Math::Vector3f_mul(&l_deltaPositionDirection_worldSpace, _Math::Vector3f_dot(&l_deltaPosition, &l_deltaPositionDirection_worldSpace) / _Math::Vector3f_length(&l_deltaPositionDirection_worldSpace), &l_deltaPosition);

							}

							_Math::Vector3f_add(&(l_transformComponent)->Transform.LocalPosition, &l_deltaPosition, &l_deltaPosition);
							_Math::Transform_setLocalPosition(&(l_transformComponent)->Transform, l_deltaPosition);
						}
					}
				}
			}


			_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
			{
				_ECS::MeshRendererBound* l_meshRendererBound = _ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entitySelection->SelectedEntity);
				// _Render::Gizmo_drawTransform(p_entitySelection->RenderInterface->Gizmo, &(l_selectedEntityTransform)->Transform);
				_Math::Vector3f l_color = { 1.0f, 1.0f, 1.0f };
				// _Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &(l_meshRendererBound)->BoundingBox, _Math::Transform_getLocalToWorldMatrix_ref(&(l_selectedEntityTransform)->Transform), true, &l_color);
			}

			// In order for the transform gimo to always have the same visible size, we fix it's z clip space position.
			{
				_ECS::TransformComponent* l_transformGizmotransform = p_entitySelection->TransformGizmoV2.TransformGizoEntity;
				if (l_transformGizmotransform)
				{
					_Math::Vector3f l_transformGizmoLocalPosition;
					{
						_Math::Vector3f l_selectedEntityTransformPosition = _Math::Transform_getWorldPosition(&l_selectedEntityTransform->Transform);
						_Math::Matrix4x4f l_worldToClipMatrix, l_clipToWorldMatrix;
						_ECS::Camera_worldToClipMatrix(l_activeCamera, &l_worldToClipMatrix);
						_Math::Matrixf4x4_inv(&l_worldToClipMatrix, &l_clipToWorldMatrix);

						_Math::Vector3f l_selectedEntityTransformClip;
						_Math::Matrix4x4f_worldToClip(&l_worldToClipMatrix, &l_selectedEntityTransformPosition, &l_selectedEntityTransformClip);
						l_selectedEntityTransformClip.z = 0.99f;
						_Math::Matrix4x4f_clipToWorld(&l_clipToWorldMatrix, &l_selectedEntityTransformClip, &l_transformGizmoLocalPosition);
					}
					_Math::Transform_setLocalPosition(&l_transformGizmotransform->Transform, l_transformGizmoLocalPosition);

					_Math::Transform_setLocalRotation(&l_transformGizmotransform->Transform, _Math::Transform_getWorldRotation(&l_selectedEntityTransform->Transform));
				}
			}
		}

	};




	_ECS::TransformComponent* transformGizmoV2_allocArrow(_ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface, _Math::Vector4f* p_color)
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
			l_transformInitInfo.LocalRotation = _Math::Quaternionf_identity();
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
				{_Render::MATERIALINSTANCE_COLOR, p_color}
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

	void TransformGizmoV2_alloc(TransformGizmo* p_transformGizmo, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface)
	{
		_ECS::Entity* l_transformGizmo = _ECS::Entity_alloc();
		{
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_transformGizmo);
			_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			p_transformGizmo->TransformGizoEntity = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, 1.0f, -10.0f };
			l_transformInitInfo.LocalRotation = _Math::Quaternionf_identity();
			l_transformInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };

			_ECS::TransformComponent_init(p_transformGizmo->TransformGizoEntity, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_transformGizmo, p_transformGizmo->TransformGizoEntity, p_ecs);
		}

		{
			_Math::Vector4f l_rightColor = { 1.0f, 0.0f, 0.0f, 1.0f };
			p_transformGizmo->RightArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &l_rightColor);
		}
		{
			_Math::Vector4f l_upColor = { 0.0f, 1.0f, 0.0f, 1.0f };
			p_transformGizmo->UpArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &l_upColor);
		}
		{
			_Math::Vector4f l_forwardColor = { 0.0f, 0.0f, 1.0f, 1.0f };
			p_transformGizmo->ForwardArrow = transformGizmoV2_allocArrow(p_ecs, p_renderInterface, &l_forwardColor);
		}

		_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->ForwardArrow->Transform);
		_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->RightArrow->Transform);
		_Math::Transform_addChild(&p_transformGizmo->TransformGizoEntity->Transform, &p_transformGizmo->UpArrow->Transform);

		{
			_Math::Quaternionf l_rightQuaternion;
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, M_PI * 0.5f, 0.0f }, &l_rightQuaternion);
			_Math::Transform_setLocalRotation(&p_transformGizmo->RightArrow->Transform, l_rightQuaternion);
		}
		{
			_Math::Quaternionf l_upQuaternion;
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ -M_PI * 0.5f, 0.0f, 0.0f }, &l_upQuaternion);
			_Math::Transform_setLocalRotation(&p_transformGizmo->UpArrow->Transform, l_upQuaternion);
		}
		{
			_Math::Quaternionf l_forwardQuaternion = _Math::Quaternionf_identity();
			_Math::Transform_setLocalRotation(&p_transformGizmo->ForwardArrow->Transform, l_forwardQuaternion);
		}

		//Plane instance
		{
			_Math::Box l_planeBox;
			l_planeBox.Center = { 0.0f, 0.0f, 0.0f };
			l_planeBox.Extend = { FLT_MAX, 0.0f, FLT_MAX };

			p_transformGizmo->TransformGizmoSelectedArrayPlane.Box = l_planeBox;

			p_transformGizmo->TransformGizmoSelectedArrayPlane.Collider.Box = &p_transformGizmo->TransformGizmoSelectedArrayPlane.Box;
			p_transformGizmo->TransformGizmoSelectedArrayPlane.Collider.Transform = &p_transformGizmo->TransformGizmoSelectedArrayPlane.Transform;
			_Math::Transform_setLocalScale(&p_transformGizmo->TransformGizmoSelectedArrayPlane.Transform, _Math::Vector3f{ 1.0f, 1.0f, 1.0f });
		}
	}


}