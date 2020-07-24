#include "EntitySelection.h"

#include "Editor/GameEngineEditor.h"
#include "GameEngineApplicationInterface.h"

#include "Algorithm/Compare/CompareAlgorithmT.hpp"

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

		if (!p_entitySelection->SelectedEntity)
		{
			if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
			{
				_Math::Vector2f l_screenPoint = { p_entitySelection->Input->InputMouse.ScreenPosition.x, p_entitySelection->Input->InputMouse.ScreenPosition.y };
				_Math::Segment l_ray;

				_ECS::Camera_buildWorldSpaceRay(_ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem), &l_screenPoint, &l_ray);

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
				if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED))
				{
					_ECS::TransformComponent* l_transformComponent = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
					_Math::Vector2d l_zero = { 0.0f, 0.0f };
					if (!_Math::Vector2d_equals(&p_entitySelection->Input->InputMouse.MouseDelta, &l_zero))
					{
						_Math::Vector3f l_deltaPosition = { p_entitySelection->Input->InputMouse.MouseDelta.x, p_entitySelection->Input->InputMouse.MouseDelta.y, 0.0f };
						_Math::Vector3f_mul(&l_deltaPosition, -p_entitySelection->Input->InputMouse.MouseSentitivityperPixel, &l_deltaPosition);
						_Math::Vector3f_add(&(l_transformComponent)->Transform.LocalPosition, &l_deltaPosition, &l_deltaPosition);
						_Math::Transform_setLocalPosition(&(l_transformComponent)->Transform, l_deltaPosition);
					}
				}
			}


			_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
			{
				_ECS::MeshRendererBound* l_meshRendererBound = _ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entitySelection->SelectedEntity);
				// _Render::Gizmo_drawTransform(p_entitySelection->RenderInterface->Gizmo, &(l_selectedEntityTransform)->Transform);
				_Math::Vector3f l_color = { 1.0f, 1.0f, 1.0f };
				_Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &(l_meshRendererBound)->BoundingBox, _Math::Transform_getLocalToWorldMatrix_ref(&(l_selectedEntityTransform)->Transform), true, &l_color);
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
						_ECS::Camera_worldToClipMatrix(_ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem), &l_worldToClipMatrix);
						_Math::Matrixf4x4_inv(&l_worldToClipMatrix, &l_clipToWorldMatrix);

						_Math::Vector3f l_selectedEntityTransformClip;
						_Math::Matrix4x4f_worldToClip(&l_worldToClipMatrix, &l_selectedEntityTransformPosition, &l_selectedEntityTransformClip);
						//TODO -> Why do we have to set such a high z value to get the transform at a decent size ?
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
	}


}