#include "EntitySelection.h"

#include "Editor/GameEngineEditor.h"
#include "GameEngineApplicationInterface.h"

#include "Algorithm/Compare/CompareAlgorithmT.hpp"

#include "Math/Segment/Segment.h"
#include "Math/Vector/VectorMath.h"

#include "Input/Input.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
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
	void entitySelection_allocTransformGizmo(EntitySelection* p_entitySelection);

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor)
	{
		p_entitySelection->ECS = p_gameEngineEditor->GameEngineApplicationInterface->ECS;
		p_entitySelection->Input = p_gameEngineEditor->GameEngineApplicationInterface->Input;
		p_entitySelection->RenderInterface = p_gameEngineEditor->GameEngineApplicationInterface->RenderInterface;
		p_entitySelection->PhysicsInterface = p_gameEngineEditor->GameEngineApplicationInterface->PhysicsInterface;
		p_entitySelection->SelectedEntity = nullptr;

		entitySelection_allocTransformGizmo(p_entitySelection);
	};

	void EntitySelection_update(EntitySelection* p_entitySelection)
	{
		if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED_THIS_FRAME))
		{
			_ECS::CameraSystem* l_cameraSystem = (_ECS::CameraSystem*)_ECS::SystemContainerV2_getSystem(&p_entitySelection->ECS->SystemContainerV2, &_ECS::CameraSystemKey);
			_Math::Vector2f l_screenPoint = { p_entitySelection->Input->InputMouse.ScreenPosition.x, p_entitySelection->Input->InputMouse.ScreenPosition.y };
			_Math::Segment l_ray;

			_ECS::CameraSystem_buildWorldSpaceRay(_ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem), &l_screenPoint, &l_ray);

			_Physics::RaycastHit l_hit;
			if (_Physics::RayCast(p_entitySelection->PhysicsInterface->World, &l_ray.Begin, &l_ray.End, &l_hit))
			{
				_ECS::TransformComponent* l_transformComponent = _ECS::TransformComponent_castFromTransform(l_hit.Collider->Transform);
				p_entitySelection->SelectedEntity = l_transformComponent->ComponentHeader.AttachedEntity;
			}
		}
		else if (_Input::Input_getState(p_entitySelection->Input, _Input::InputKey::MOUSE_BUTTON_1, _Input::KeyStateFlag::PRESSED))
		{
			if (p_entitySelection->SelectedEntity)
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
		

		if (p_entitySelection->SelectedEntity)
		{
			_ECS::TransformComponent* l_selectedEntityTransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->SelectedEntity);
			{
				_ECS::MeshRendererBound* l_meshRendererBound = _ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entitySelection->SelectedEntity);
				_Render::Gizmo_drawTransform(p_entitySelection->RenderInterface->Gizmo, &(l_selectedEntityTransform)->Transform);
				_Math::Vector3f l_color = { 1.0f, 1.0f, 1.0f };
				_Render::Gizmo_drawBox(p_entitySelection->RenderInterface->Gizmo, &(l_meshRendererBound)->BoundingBox, _Math::Transform_getLocalToWorldMatrix_ref(&(l_selectedEntityTransform)->Transform), true, &l_color);
			}

			{
				_ECS::TransformComponent* l_transformGizmotransform = _ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entitySelection->TransformGizmo);
				_Math::Transform_setLocalPosition(&l_transformGizmotransform->Transform, l_selectedEntityTransform->Transform.LocalPosition);
			}
		}

	};

	void entitySelection_allocTransformGizmo(EntitySelection* p_entitySelection)
	{
		_ECS::ECS* l_ecs = p_entitySelection->ECS;

		{
			p_entitySelection->TransformGizmo = _ECS::Entity_alloc();
			_ECS::ECSEventMessage* l_entityCreationMessage = _ECS::ECSEventMessage_addEntity_alloc(&p_entitySelection->TransformGizmo);
			_ECS::ECSEventQueue_pushMessage(&l_ecs->EventQueue, &l_entityCreationMessage);
		}
		{
			_ECS::TransformComponent* l_transform = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, 1.0f, -10.0f };
			l_transformInitInfo.LocalRotation = _Math::Quaternionf_identity();
			l_transformInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };

			_ECS::TransformComponent_init(l_transform, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(p_entitySelection->TransformGizmo, l_transform, l_ecs);
		}
		{
			_ECS::MeshRenderer* l_meshRenderer = _ECS::ComponentT_alloc<_ECS::MeshRenderer>();
			_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
			_Render::MaterialUniqueKey l_materialKey{};
			l_materialKey.FragmentShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoFragment.spv";
			l_materialKey.VertexShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/3DGizmoVertex.spv";
			l_meshRendererInitInfo.MaterialUniqueKey = &l_materialKey;
			_Math::Vector4f l_color = { 0.0f, 1.0f, 0.0f, 1.0f };
			l_meshRendererInitInfo.InputParameters = {
				{_Render::MATERIALINSTANCE_MESH_KEY, "E:/GameProjects/GameEngine/Assets/Models/ForwardArrow.obj"},
				// {_Render::MATERIALINSTANCE_TEXTURE_KEY, "E:/GameProjects/GameEngine/Assets/Textures/red.png"},
				{_Render::MATERIALINSTANCE_COLOR, &l_color}
			};

			_ECS::MeshRenderer_init(l_meshRenderer, p_entitySelection->RenderInterface, &l_meshRendererInitInfo);
			_ECS::EntityT_addComponentDeferred(p_entitySelection->TransformGizmo, l_meshRenderer, l_ecs);
		}
	};
}