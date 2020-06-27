#include "SandboxApplication.h"

#include "GameEngineApplication.h"
#include "GameEngineEditor.h"

#include "DataStructures/String.h"
#include "DataStructures/StringConverterPrimitives.h"

#include "Math/Segment/Segment.h"

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

_ECS::Entity* l_parent;
_ECS::Entity* l_child;
_ECS::Entity* l_child2;

void SandboxApplication_update(float p_delta)
{
	if (!HasAlreadyUpdated)
	{
		{
			l_cameraEntity = _ECS::Entity_alloc(&App->ECS);
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_cameraEntity);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::CameraType, sizeof(_ECS::Camera));
			_ECS::Camera* l_camera = (_ECS::Camera*)l_component->Child;
			_ECS::Camera_init(l_camera, l_component, &App->Render.RenderInterface);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_cameraEntity, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
			_ECS::TransformComponent* l_transform = (_ECS::TransformComponent*)l_component->Child;

			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 3.0f, 1.5f, -3.0f };
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.30f, -0.70f, 0.04f }, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_cameraEntity, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			l_parent = _ECS::Entity_alloc(&App->ECS);
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_parent);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererType, sizeof(_ECS::MeshRenderer));
			_ECS::MeshRenderer* l_meshRenderer = (_ECS::MeshRenderer*)l_component->Child;

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
			l_meshRendererInitInfo.AssociatedComponent = l_component;
			l_meshRendererInitInfo.MaterialUniqueKey = &l_materialKey;

			_ECS::MeshRenderer_init(l_meshRenderer, &App->Render.RenderInterface, &l_meshRendererInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_parent, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		_ECS::TransformComponent* l_parentTransform;

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
			l_parentTransform = (_ECS::TransformComponent*)l_component->Child;

			{
				_Core::String* l_str = _Log::MyLog_AllocateString(&App->Log);
				_Core::String_appendPointer(l_str, l_parentTransform);
				MYLOG_PUSH(&App->Log, _Log::INFO, l_str->c_str());
			}

			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, 0.0f };
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_parent, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererBoundType, sizeof(_ECS::MeshRendererBound));

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_parent, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformRotateType, sizeof(_ECS::TransformRotate));
			_ECS::TransformRotate* l_transformRotate = (_ECS::TransformRotate*)l_component->Child;
			l_transformRotate->Speed = 1.0f;
			l_transformRotate->Axis = _Math::UP;

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_parent, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			l_child = _ECS::Entity_alloc(&App->ECS);
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_child);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererType, sizeof(_ECS::MeshRenderer));
			_ECS::MeshRenderer* l_meshRenderer = (_ECS::MeshRenderer*)l_component->Child;

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
			l_meshRendererInitInfo.AssociatedComponent = l_component;

			_ECS::MeshRenderer_init(l_meshRenderer, &App->Render.RenderInterface, &l_meshRendererInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		_ECS::TransformComponent* l_childTransform;
		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
			l_childTransform = (_ECS::TransformComponent*)l_component->Child;

			// _Log::LogInstance->CoreLogger->info((void*)l_transform);

			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.15f, -0.15f, 0.0f };
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = { 1.0f ,1.0f ,1.0f };
			_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformRotateType, sizeof(_ECS::TransformRotate));
			_ECS::TransformRotate* l_transformRotate = (_ECS::TransformRotate*)l_component->Child;
			l_transformRotate->Speed = 1.0f;
			l_transformRotate->Axis = _Math::UP;

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererBoundType, sizeof(_ECS::MeshRendererBound));

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		_Math::Transform_addChild(
			&l_parentTransform->Transform,
			&l_childTransform->Transform
		);

		{
			l_child2 = _ECS::Entity_alloc(&App->ECS);
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_child2);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererType, sizeof(_ECS::MeshRenderer));
			_ECS::MeshRenderer* l_meshRenderer = (_ECS::MeshRenderer*)l_component->Child;

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
			l_meshRendererInitInfo.AssociatedComponent = l_component;

			_ECS::MeshRenderer_init(l_meshRenderer, &App->Render.RenderInterface, &l_meshRendererInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child2, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		_ECS::TransformComponent* l_child2Transform;
		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformComponentType, sizeof(_ECS::TransformComponent));
			l_child2Transform = (_ECS::TransformComponent*)l_component->Child;

			// _Log::LogInstance->CoreLogger->info((void*)l_transform);

			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.15f, -0.15f, 0.0f };
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 0.0f, 0.0f, 0.0f }, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_component, &l_transformInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child2, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}


		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererBoundType, sizeof(_ECS::MeshRendererBound));

			auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&l_child2, &l_component);
			_ECS::ECSEventQueue_pushMessage(&App->ECS.EventQueue, &l_addComponentMessage);
		}

		_Math::Transform_addChild(
			&l_childTransform->Transform,
			&l_child2Transform->Transform
		);


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
		// _Render::Gizmo_drawTransform(&App->Render.Gizmo, &((_ECS::TransformComponent*)_ECS::Entity_getComponent(l_child, _ECS::TransformComponentType)->Child)->Transform);
		// _Render::Gizmo_drawTransform(&App->Render.Gizmo, &((_ECS::TransformComponent*)_ECS::Entity_getComponent(l_child2, _ECS::TransformComponentType)->Child)->Transform);


#if comment

		{
			_Math::Vector3f l_begin = { -10.0f, -10.0f, 00.0f };
			_Math::Vector3f l_end = { 10.0f, 10.0f, 00.0f };
			// _Render::Gizmo_drawLine(&App->Render.Gizmo, &l_begin, &l_end);

			_Core::VectorT<_Physics::RaycastHit> l_intersectionPoints;
			l_intersectionPoints.alloc();
			{
				_Physics::RayCastAll(&App->Physics.World, &l_begin, &l_end, &l_intersectionPoints);

				for (size_t i = 0; i < l_intersectionPoints.size(); i++)
				{
					_Render::Gizmo_drawPoint(&App->Render.Gizmo, &l_intersectionPoints.at(i)->HitPoint);
				}
			}
			l_intersectionPoints.free();

		}
#endif

		{
			_ECS::SystemV2* l_cameraSystem = _ECS::SystemContainer_getSystem(&App->ECS.SystemContainer, &_ECS::CameraSystemKey);
			_Math::Vector2f l_screenPoint = { App->Input.InputMouse.ScreenPosition.x, App->Input.InputMouse.ScreenPosition.y };
			_Math::Segment l_ray;

			_ECS::CameraSystem_buildWorldSpaceRay(_ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem), &l_screenPoint, &l_ray);

			/*
			{
				_Core::String l_logMsg;
				l_logMsg.alloc(0);
				l_logMsg.append("begin : ");
				_Math::Vector3f_toString(&l_ray.Begin, &l_logMsg);
				l_logMsg.append(" , end : ");
				_Math::Vector3f_toString(&l_ray.End, &l_logMsg);
				MYLOG_PUSH(&App->Log, _Log::INFO, l_logMsg.c_str());
				l_logMsg.free();
			}
			*/

			_Render::Gizmo_drawLine(&App->Render.Gizmo, &l_ray.Begin, &l_ray.End);

			_Physics::RaycastHit l_hit;
			if (_Physics::RayCast(&App->Physics.World, &l_ray.Begin, &l_ray.End, &l_hit))
			{
				_Render::Gizmo_drawPoint(&App->Render.Gizmo, &l_hit.HitPoint);
			}
		}
	}

}
