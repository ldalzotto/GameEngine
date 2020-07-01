#include "SandboxApplication.h"

#include "GameEngineApplication.h"
#include "GameEngineEditor.h"

#include "DataStructures/String.h"
#include "DataStructures/StringConverterPrimitives.h"

#include "Math/Segment/Segment.h"
#include "Math/Math.h"

#include "ECS/Entity.h"
#include "ECS/ComponentMacros.h"

#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Transform/TransformRotateSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"
#include "ECS_Impl/Systems/MeshDraw/MeshRendererBoundSystem.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/SystemV2Factory.h"

#include "Physics/World/RayCast.h"
#include "Physics/World/Collider/BoxCollider.h"

#include "MyLog/MyLog.h"

#include "Math/Quaternion/QuaternionMath.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace _GameEngine;

void SandboxApplication_update(float p_delta);

int main()
{
	App = app_alloc(SandboxApplication_update);
	App_Editor = _GameEngineEditor::GameEngineEditor_alloc(&App->GameEngineApplicationInterface);

	try
	{
		app_run(App);
	}
	catch (const std::exception& e)
	{
		_Log::MyLog_pushLog(&App->Log, _Log::ERROR, __FILE__, __LINE__, (char*)e.what());
		_GameEngineEditor::GameEngineEditor_free(&App_Editor, &App->GameEngineApplicationInterface);
		app_free(App);
		return EXIT_FAILURE;
	}
	catch (...)
	{
		_Log::MyLog_pushLog(&App->Log, _Log::ERROR, __FILE__, __LINE__, "Unexpected Error");
		_GameEngineEditor::GameEngineEditor_free(&App_Editor, &App->GameEngineApplicationInterface);
		app_free(App);
		return EXIT_FAILURE;
	}

	_GameEngineEditor::GameEngineEditor_free(&App_Editor, &App->GameEngineApplicationInterface);
	app_free(App);
	return EXIT_SUCCESS;
};

bool HasAlreadyUpdated = false;

_ECS::Entity* l_cameraEntity;

_ECS::Entity* l_sceneModelsRootEntity;
_ECS::TransformComponent* l_sceneModelsRootTransform;

_ECS::Entity* l_rayEntity;
_ECS::TransformComponent* l_rayEntityTransform;
_ECS::Entity* l_rayBeginEntity;
_ECS::TransformComponent* l_rayBeginEntityTransform;
_ECS::Entity* l_rayEndEntity;
_ECS::TransformComponent* l_rayEndEntityTransform;

struct SandboxApplicationCubeCreationInfo
{
	_Math::Vector3f LocalPosition;
	_Math::Quaternionf LocalRotation;
	_Math::Vector3f LocalScale;
	bool WithMeshBound;
	_ECS::MeshRendererInitInfo* MeshRendererInitInfo;
};

void SandboxApplication_createCubeEntity(SandboxApplicationCubeCreationInfo* p_sandboxCubeCreationInfo, _ECS::Entity** out_entity, _ECS::TransformComponent** out_entitytransform)
{

	{
		*out_entity = _ECS::Entity_alloc(&App->ECS);
		auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(out_entity);
		_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
	}

	{
		COMPONENT_ALLOC(_ECS::TransformComponent, l_component);
		COMPONENT_GET_CHILD(_ECS::TransformComponent, l_component, l_transformComponent);

		_ECS::TransformInitInfo l_transformInitInfo{};
		l_transformInitInfo.LocalPosition = p_sandboxCubeCreationInfo->LocalPosition;
		l_transformInitInfo.LocalRotation = p_sandboxCubeCreationInfo->LocalRotation;
		l_transformInitInfo.LocalScale = p_sandboxCubeCreationInfo->LocalScale;
		_ECS::TransformComponent_init(l_component, &l_transformInitInfo);
		*out_entitytransform = l_transformComponent;

		auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(out_entity, &l_component);
		_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
	}

	if (p_sandboxCubeCreationInfo->MeshRendererInitInfo)
	{
		COMPONENT_ALLOC(_ECS::MeshRenderer, l_component);
		COMPONENT_GET_CHILD(_ECS::MeshRenderer, l_component, l_meshRenderer);
		p_sandboxCubeCreationInfo->MeshRendererInitInfo->AssociatedComponent = l_component;
		_ECS::MeshRenderer_init(l_meshRenderer, &App->Render.RenderInterface, p_sandboxCubeCreationInfo->MeshRendererInitInfo);

		auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(out_entity, &l_component);
		_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
	}

	if (p_sandboxCubeCreationInfo->WithMeshBound)
	{
		COMPONENT_ALLOC(_ECS::MeshRendererBound, l_component);
		auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(out_entity, &l_component);
		_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
	}

};

struct CubeCrossCreationInfo
{
	_Math::Vector3f LocalPosition;
	_Math::Quaternionf LocalRotation;
	_Math::Vector3f LocalScale;
	_ECS::TransformComponent* Parent;
	_Math::Vector3f* RotationAxis;
};

void SanboxApplication_createCubeCross(CubeCrossCreationInfo* p_cubeCrossCreationInfo)
{
	_ECS::Entity* l_parentEntity;
	_ECS::TransformComponent* l_parentEntityTransform;

	_ECS::Entity* l_upLeftEntity;
	_ECS::TransformComponent* l_upLeftEntityTransform;

	_ECS::Entity* l_upRightEntity;
	_ECS::TransformComponent* l_upRightEntityTransform;

	_ECS::Entity* l_bottomLeftEntity;
	_ECS::TransformComponent* l_bottomLeftEntityTransform;

	_ECS::Entity* l_bottomRightEntity;
	_ECS::TransformComponent* l_bottomRightEntityTransform;

	{
		SandboxApplicationCubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = p_cubeCrossCreationInfo->LocalPosition;
		l_parentCreationInfo.LocalRotation = p_cubeCrossCreationInfo->LocalRotation;
		l_parentCreationInfo.LocalScale = p_cubeCrossCreationInfo->LocalScale;
		SandboxApplication_createCubeEntity(&l_parentCreationInfo, &l_parentEntity, &l_parentEntityTransform);
	}


	std::unordered_map<std::string, void*> l_defaultMaterialInput
	{
		{_Render::MATERIALINSTANCE_MESH_KEY, "E:/GameProjects/GameEngine/Assets/Models/Cube.obj"},
		{_Render::MATERIALINSTANCE_TEXTURE_KEY, "E:/GameProjects/GameEngine/Assets/Textures/MinecraftDirtV2.png"}
	};

	_Render::MaterialUniqueKey l_materialKey{};
	l_materialKey.VertexShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialVertex.spv";
	l_materialKey.FragmentShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialFragment.spv";

	_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
	l_meshRendererInitInfo.InputParameters = l_defaultMaterialInput;
	l_meshRendererInitInfo.MaterialUniqueKey = &l_materialKey;


	{
		SandboxApplicationCubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { -1.0f, 1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _Math::Quaternionf_identity();
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &l_meshRendererInitInfo;
		SandboxApplication_createCubeEntity(&l_parentCreationInfo, &l_upLeftEntity, &l_upLeftEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_upLeftEntityTransform->Transform
		);
	}

	{
		SandboxApplicationCubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { 1.0f, 1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _Math::Quaternionf_identity();
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &l_meshRendererInitInfo;
		SandboxApplication_createCubeEntity(&l_parentCreationInfo, &l_upRightEntity, &l_upRightEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_upRightEntityTransform->Transform
		);
	}

	{
		SandboxApplicationCubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { -1.0f, -1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _Math::Quaternionf_identity();
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &l_meshRendererInitInfo;
		SandboxApplication_createCubeEntity(&l_parentCreationInfo, &l_bottomLeftEntity, &l_bottomLeftEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_bottomLeftEntityTransform->Transform
		);
	}

	{
		SandboxApplicationCubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { 1.0f, -1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _Math::Quaternionf_identity();
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &l_meshRendererInitInfo;
		SandboxApplication_createCubeEntity(&l_parentCreationInfo, &l_bottomRightEntity, &l_bottomRightEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_bottomRightEntityTransform->Transform
		);
	}

	if (p_cubeCrossCreationInfo->Parent)
	{
		_Math::Transform_addChild(
			&p_cubeCrossCreationInfo->Parent->Transform,
			&l_parentEntityTransform->Transform
		);
	}

	if (p_cubeCrossCreationInfo->RotationAxis)
	{
		COMPONENT_ALLOC(_ECS::TransformRotate, l_component);
		COMPONENT_GET_CHILD(_ECS::TransformRotate, l_component, l_transformRotate);
		l_transformRotate->Speed = .5f;
		l_transformRotate->Axis = *p_cubeCrossCreationInfo->RotationAxis;
		_ECS::Entity_addComponentDeferred(l_parentEntity, l_component, &App->ECS);
	}
};

void SandboxApplication_update(float p_delta)
{
	if (!HasAlreadyUpdated)
	{
		// Camera
		{
			{
				l_cameraEntity = _ECS::Entity_alloc(&App->ECS);
				auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_cameraEntity);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
			}

			{
				COMPONENT_ALLOC(_ECS::Camera, l_component);
				COMPONENT_GET_CHILD(_ECS::Camera, l_component, l_camera);
				_ECS::Camera_init(l_camera, l_component, &App->Render.RenderInterface);

				auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_cameraEntity, &l_component);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
			}

			{
				COMPONENT_ALLOC(_ECS::TransformComponent, l_component);
				_ECS::TransformInitInfo l_transformInitInfo{};
				l_transformInitInfo.LocalPosition = { 9.0f, 9.0f, 9.0f };
				_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ (M_PI * 0.20f), M_PI + (M_PI * 0.25f), 0.0f }, &l_transformInitInfo.LocalRotation);
				l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
				_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

				auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_cameraEntity, &l_component);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
			}
		}

		// Ray
		{
			{
				l_rayEntity = _ECS::Entity_alloc(&App->ECS);
				auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rayEntity);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
			}

			{

				_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
				l_rayEntityTransform = (_ECS::TransformComponent*)l_component->Child;

				_ECS::TransformInitInfo l_transformInitInfo{};
				l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, -0.0f };
				_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
				l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
				_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

				auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_rayEntity, &l_component);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
			}

			/*
			{
				_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformRotateType, sizeof(_ECS::TransformRotate));
				_ECS::TransformRotate* l_transformRotate = (_ECS::TransformRotate*)l_component->Child;
				l_transformRotate->Speed = 1.0f;
				l_transformRotate->Axis = _Math::RIGHT;

				auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_rayEntity, &l_component);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
			}
			*/

			//Ray Begin
			{
				{
					l_rayBeginEntity = _ECS::Entity_alloc(&App->ECS);
					auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rayBeginEntity);
					_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
				}

				{
					_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
					l_rayBeginEntityTransform = (_ECS::TransformComponent*)l_component->Child;

					_ECS::TransformInitInfo l_transformInitInfo{};
					l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, -100.0f };
					_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
					l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
					_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

					auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_rayBeginEntity, &l_component);
					_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);

				}
			}

			//Ray End
			{
				{
					l_rayEndEntity = _ECS::Entity_alloc(&App->ECS);
					auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rayEndEntity);
					_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
				}

				{
					_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
					l_rayEndEntityTransform = (_ECS::TransformComponent*)l_component->Child;

					_ECS::TransformInitInfo l_transformInitInfo{};
					l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, 100.0f };
					_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
					l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
					_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

					auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_rayEndEntity, &l_component);
					_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);

				}
			}

			_Math::Transform_addChild(&l_rayEntityTransform->Transform, &l_rayBeginEntityTransform->Transform);
			_Math::Transform_addChild(&l_rayEntityTransform->Transform, &l_rayEndEntityTransform->Transform);
		}

		// Scene root
		{
			{
				l_sceneModelsRootEntity = _ECS::Entity_alloc(&App->ECS);
				auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_sceneModelsRootEntity);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
			}

			{

				_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
				l_sceneModelsRootTransform = (_ECS::TransformComponent*)l_component->Child;

				_ECS::TransformInitInfo l_transformInitInfo{};
				l_transformInitInfo.LocalPosition = { 0.0f, 0.0f, 0.0f };
				_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
				l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
				_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

				auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_sceneModelsRootEntity, &l_component);
				_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);

			}
		}
		_Math::Transform_addChild(
			&l_sceneModelsRootTransform->Transform,
			&l_rayEntityTransform->Transform
		);

		// Cubes
		{
			{
				_Math::Vector3f l_rotation = { 1.0f, 1.0f, 1.0f };
				CubeCrossCreationInfo l_cubeCrossCreationInfo{};
				l_cubeCrossCreationInfo.Parent = l_sceneModelsRootTransform;
				l_cubeCrossCreationInfo.LocalPosition = { 1.0f, 0.0f, 0.0f };
				l_cubeCrossCreationInfo.LocalRotation = _Math::Quaternionf_identity();
				l_cubeCrossCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
				l_cubeCrossCreationInfo.RotationAxis = &l_rotation;
				SanboxApplication_createCubeCross(&l_cubeCrossCreationInfo);
			}

			{
				_Math::Vector3f l_rotation = { 1.0f, 1.0f, 0.0f };
				CubeCrossCreationInfo l_cubeCrossCreationInfo{};
				l_cubeCrossCreationInfo.Parent = l_sceneModelsRootTransform;
				l_cubeCrossCreationInfo.LocalPosition = { 0.0f, -1.0f, 2.0f };
				_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, M_PI * 0.5f, 0.0f }, &l_cubeCrossCreationInfo.LocalRotation);
				l_cubeCrossCreationInfo.LocalScale = { 2.0f, 1.0f, 1.0f };
				l_cubeCrossCreationInfo.RotationAxis = &l_rotation;
				SanboxApplication_createCubeCross(&l_cubeCrossCreationInfo);
			}
		}


		// System initialization
		{
			_ECS::SystemV2AllocInfo l_systemAllocInfo{};
			_ECS::TransformRotateSystemV2_init(&l_systemAllocInfo, &App->ECS);
			_ECS_Impl::SystemV2Factory_allocSystemV2(&l_systemAllocInfo, &App->UpdateSequencer);

			l_systemAllocInfo = {};
			_ECS::MeshDrawSystemV2_init(&l_systemAllocInfo, &App->ECS);
			_ECS_Impl::SystemV2Factory_allocSystemV2(&l_systemAllocInfo, &App->UpdateSequencer);

			l_systemAllocInfo = {};
			_ECS::CameraSystem_init(&l_systemAllocInfo, &App->ECS);
			_ECS_Impl::SystemV2Factory_allocSystemV2(&l_systemAllocInfo, &App->UpdateSequencer);

			l_systemAllocInfo = {};
			_ECS::MeshRendererBoundSystem_init(&l_systemAllocInfo, &App->ECS, &App->Physics.PhysicsInterface);
			_ECS_Impl::SystemV2Factory_allocSystemV2(&l_systemAllocInfo, &App->UpdateSequencer);
		}

		HasAlreadyUpdated = true;
	}
	else
	{
#ifndef comment
		{
			_Math::Vector3f l_rootCenter = { 0.0f, 0.0f, 0.0f };
			_Render::Gizmo_drawTransform(&App->Render.Gizmo, &l_rootCenter, &_Math::RIGHT, &_Math::UP, &_Math::FORWARD);
		}
#endif

		{
			_Math::Vector3f l_rayBeginPoint = _Math::Transform_getWorldPosition(&l_rayBeginEntityTransform->Transform);
			_Math::Vector3f l_rayEndPoint = _Math::Transform_getWorldPosition(&l_rayEndEntityTransform->Transform);
			_Math::Vector3f l_color = { 0.0f, 1.0f, 0.0f };

			_Render::Gizmo_drawLine(&App->Render.Gizmo, &l_rayBeginPoint, &l_rayEndPoint, &l_color);

			_Core::VectorT<_Physics::RaycastHit> l_hits{};
			l_hits.alloc();
			{
				_Physics::RayCastAll(&App->Physics.World, &l_rayBeginPoint, &l_rayEndPoint, &l_hits);
				for (size_t i = 0; i < l_hits.size(); i++)
				{
					_Render::Gizmo_drawPoint(&App->Render.Gizmo, &l_hits.at(i)->HitPoint, &l_color);
					_Render::Gizmo_drawBox(&App->Render.Gizmo, l_hits.at(i)->Collider->Box, _Math::Transform_getLocalToWorldMatrix_ref(l_hits.at(i)->Collider->Transform), false, &l_color);
				}
			}
			l_hits.free();
		}

		// Mouse raycast
		{
			_ECS::SystemV2* l_cameraSystem = _ECS::SystemContainer_getSystem(&App->ECS.SystemContainer, &_ECS::CameraSystemKey);
			_Math::Vector2f l_screenPoint = { App->Input.InputMouse.ScreenPosition.x, App->Input.InputMouse.ScreenPosition.y };
			_Math::Segment l_ray;

			_ECS::CameraSystem_buildWorldSpaceRay(_ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem), &l_screenPoint, &l_ray);

			_Physics::RaycastHit l_hit;
			if (_Physics::RayCast(&App->Physics.World, &l_ray.Begin, &l_ray.End, &l_hit))
			{
				_Render::Gizmo_drawPoint(&App->Render.Gizmo, &l_hit.HitPoint);
				_Render::Gizmo_drawBox(&App->Render.Gizmo, l_hit.Collider->Box, _Math::Transform_getLocalToWorldMatrix_ref(l_hit.Collider->Transform), false);
			}

		}
	}

}
