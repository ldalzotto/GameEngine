#include "Lib/IntegrationTest.h"

#include "Lib/EntityCreation.h"
#include "Lib/EntityConfigurations.h"

#include "GameEngineApplication.h"

#include "Math/Math.h"
#include "Math/Transform/Transform.h"
#include "v2/Quaternion/QuaternionMath.hpp"
#include "Math/Segment/Segment.h"

#include "ECS/ComponentT.hpp"
#include "ECS/ECSEventQueueT.hpp"
#include "ECS/EntityT.hpp"

#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"

#include "Physics/World/RayCast.h"
#include "Physics/World/Collider/BoxCollider.h"

using namespace _GameEngine::_Test;


struct TestIntTest
{
	_ECS::TransformComponent* PhysicsRayBegin;
	_ECS::TransformComponent* PhysicsRayEnd;
};

void TestInt_udpate(TestIntTest* p_test, GameEngineApplicationInterface* l_gameEngine);

struct CubeCrossCreationInfo
{
	_MathV2::Vector3<float> LocalPosition;
	_MathV2::Quaternion<float> LocalRotation;
	_MathV2::Vector3<float> LocalScale;
	_ECS::TransformComponent* Parent;
	_MathV2::Vector3<float>* RotationAxis;
};

void TestInt_createCubeCross(GameEngineApplicationInterface* l_gameEngine, CubeCrossCreationInfo* p_cubeCrossCreationInfo)
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
		CubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = p_cubeCrossCreationInfo->LocalPosition;
		l_parentCreationInfo.LocalRotation = p_cubeCrossCreationInfo->LocalRotation;
		l_parentCreationInfo.LocalScale = p_cubeCrossCreationInfo->LocalScale;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_parentEntity, &l_parentEntityTransform);
	}


	{
		CubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { -1.0f, 1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_upLeftEntity, &l_upLeftEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_upLeftEntityTransform->Transform
		);
	}

	{
		CubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { 1.0f, 1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_upRightEntity, &l_upRightEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_upRightEntityTransform->Transform
		);
	}

	{
		CubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { -1.0f, -1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_bottomLeftEntity, &l_bottomLeftEntityTransform);

		_Math::Transform_addChild(
			&l_parentEntityTransform->Transform,
			&l_bottomLeftEntityTransform->Transform
		);
	}

	{
		CubeCreationInfo l_parentCreationInfo{};
		l_parentCreationInfo.LocalPosition = { 1.0f, -1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
		l_parentCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_bottomRightEntity, &l_bottomRightEntityTransform);

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
		_ECS::TransformRotate* l_transformRotate = _ECS::ComponentT_alloc<_ECS::TransformRotate>();
		(l_transformRotate)->Speed = .5f;
		(l_transformRotate)->Axis = *(_Math::Vector3f*)p_cubeCrossCreationInfo->RotationAxis;
		_ECS::EntityT_addComponentDeferred(l_parentEntity, l_transformRotate, l_gameEngine->ECS);
	}
};



void TestInt_init(_GameEngine::GameEngineApplication* l_app, TestIntTest* p_test)
{
	
	EntityConfiguration_init();

	_ECS::Entity* l_cameraEntity;
	_ECS::TransformComponent* l_rayTransform;
	_ECS::TransformComponent* l_sceneModelsRootTransform;

	// Camera
	{
		{
			l_cameraEntity = _ECS::Entity_alloc();
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_cameraEntity);
			_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			auto l_camera = _ECS::ComponentT_alloc<_ECS::Camera>();
			_ECS::Camera_init(l_camera, &l_app->Render.RenderInterface);
			_ECS::EntityT_addComponentDeferred(l_cameraEntity, l_camera, &l_app->ECS);
		}

		{
			auto l_component = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 9.0f, 9.0f, 9.0f };
			l_transformInitInfo.LocalRotation = _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{(M_PI * 0.20f), M_PI + (M_PI * 0.25f), 0.0f});
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_component, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_cameraEntity, l_component, &l_app->ECS);
		}
	}
	// Ray
	{
		_ECS::Entity* l_rayEntity;
		{
			l_rayEntity = _ECS::Entity_alloc();
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rayEntity);
			_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::TransformComponent* l_transformComponent = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			l_rayTransform = l_transformComponent;
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, -0.0f };
			l_transformInitInfo.LocalRotation = _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, 0.0f, 0.0f});
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_transformComponent, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_rayEntity, l_transformComponent, &l_app->ECS);
		}

		_ECS::Entity* l_rayBeginEntity;
		_ECS::Entity* l_rayEndEntity;

		//Ray Begin
		{
			{
				l_rayBeginEntity = _ECS::Entity_alloc();
				auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rayBeginEntity);
				_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addEntityMessage);
			}

			{
				_ECS::TransformComponent* l_transformComponent = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
				p_test->PhysicsRayBegin = l_transformComponent;

				_ECS::TransformInitInfo l_transformInitInfo{};
				l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, -100.0f };
				l_transformInitInfo.LocalRotation = _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, 0.0f, 0.0f});
				l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
				_ECS::TransformComponent_init(l_transformComponent, &l_transformInitInfo);

				auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(&l_rayBeginEntity, l_transformComponent);
				_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addComponentMessage);

			}
		}

		//Ray End
		{
			{
				l_rayEndEntity = _ECS::Entity_alloc();
				auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_rayEndEntity);
				_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addEntityMessage);
			}

			{
				_ECS::TransformComponent* l_transformComponent = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
				p_test->PhysicsRayEnd = l_transformComponent;

				_ECS::TransformInitInfo l_transformInitInfo{};
				l_transformInitInfo.LocalPosition = { 0.0f, -0.0f, 100.0f };
				l_transformInitInfo.LocalRotation = _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, 0.0f, 0.0f});
				l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
				_ECS::TransformComponent_init(l_transformComponent, &l_transformInitInfo);

				auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(&l_rayEndEntity, l_transformComponent);
				_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addComponentMessage);

			}
		}

		_Math::Transform_addChild(&(l_rayTransform)->Transform, &(p_test->PhysicsRayBegin)->Transform);
		_Math::Transform_addChild(&(l_rayTransform)->Transform, &(p_test->PhysicsRayEnd)->Transform);
	}

	// Scene root
	{
		_ECS::Entity* l_sceneModelsRootEntity;
		{
			l_sceneModelsRootEntity = _ECS::Entity_alloc();
			auto l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_sceneModelsRootEntity);
			_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::TransformComponent* l_transformComponent = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			l_sceneModelsRootTransform = l_transformComponent;
			
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 0.0f, 0.0f, 0.0f };
			l_transformInitInfo.LocalRotation = _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, 0.0f, 0.0f});
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_transformComponent, &l_transformInitInfo);

			auto l_addComponentMessage = _ECS::ECSEventMessageT_AddComponent_alloc(&l_sceneModelsRootEntity, l_transformComponent);
			_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addComponentMessage);

		}
	}
	_Math::Transform_addChild(
		&(l_sceneModelsRootTransform)->Transform,
		&(l_rayTransform)->Transform
	);


	// Cubes
	{
		{
			_MathV2::Vector3<float> l_rotation = { 1.0f, 1.0f, 1.0f };
			CubeCrossCreationInfo l_cubeCrossCreationInfo{};
			l_cubeCrossCreationInfo.Parent = l_sceneModelsRootTransform;
			l_cubeCrossCreationInfo.LocalPosition = { 1.0f, 0.0f, 0.0f };
			l_cubeCrossCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
			l_cubeCrossCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
			l_cubeCrossCreationInfo.RotationAxis = &l_rotation;
			TestInt_createCubeCross(&l_app->GameEngineApplicationInterface, &l_cubeCrossCreationInfo);
		}

		{
			_MathV2::Vector3<float> l_rotation = { 1.0f, 1.0f, 0.0f };
			CubeCrossCreationInfo l_cubeCrossCreationInfo{};
			l_cubeCrossCreationInfo.Parent = l_sceneModelsRootTransform;
			l_cubeCrossCreationInfo.LocalPosition = { 0.0f, -1.0f, 2.0f };
			l_cubeCrossCreationInfo.LocalRotation = _MathV2::QuaternionM::fromEulerAngle(_MathV2::Vector3<float>{0.0f, M_PI * 0.5f, 0.0f});
			l_cubeCrossCreationInfo.LocalScale = { 2.0f, 1.0f, 1.0f };
			l_cubeCrossCreationInfo.RotationAxis = &l_rotation;
			TestInt_createCubeCross(&l_app->GameEngineApplicationInterface, &l_cubeCrossCreationInfo);
		}
	}


	_Core::CallbackT<TestIntTest, _GameEngine::GameEngineApplicationInterface> l_testUpdate = { TestInt_udpate, p_test };
	_Core::ObserverT_register(&l_app->EndOfUpdate, (_Core::CallbackT<void, _GameEngine::GameEngineApplicationInterface>*)&l_testUpdate);
	
}

void TestInt_udpate(TestIntTest* p_test, GameEngineApplicationInterface* l_interface)
{
#ifndef comment
	{
		_Math::Vector3f l_rootCenter = { 0.0f, 0.0f, 0.0f };
		_Render::Gizmo_drawTransform(l_interface->RenderInterface->Gizmo, &l_rootCenter, &_Math::RIGHT, &_Math::UP, &_Math::FORWARD);
	}
#endif

	{
		_MathV2::Vector3<float> l_rayBeginPoint = _Math::Transform_getWorldPosition(&(p_test->PhysicsRayBegin)->Transform);
		_MathV2::Vector3<float> l_rayEndPoint = _Math::Transform_getWorldPosition(&(p_test->PhysicsRayEnd)->Transform);
		_Math::Vector3f l_color = { 0.0f, 1.0f, 0.0f };

		_Render::Gizmo_drawLine(l_interface->RenderInterface->Gizmo, (_Math::Vector3f*)&l_rayBeginPoint, (_Math::Vector3f*) &l_rayEndPoint, &l_color);

		_Core::VectorT<_Physics::RaycastHit> l_hits{};
		_Core::VectorT_alloc(&l_hits, 0);
		{
			_Physics::RayCastAll(l_interface->PhysicsInterface->World, l_rayBeginPoint, l_rayEndPoint, &l_hits);
			_Core::VectorIteratorT<_Physics::RaycastHit> l_hitsIt = _Core::VectorT_buildIterator(&l_hits);
			while (_Core::VectorIteratorT_moveNext(&l_hitsIt))
			{
				_Render::Gizmo_drawPoint(l_interface->RenderInterface->Gizmo, (_Math::Vector3f*)&l_hitsIt.Current->HitPoint, &l_color);
				_Render::Gizmo_drawBox(l_interface->RenderInterface->Gizmo, l_hitsIt.Current->Collider->Box, 
					(_Math::Matrix4x4f*)_Math::Transform_getLocalToWorldMatrix_ref(l_hitsIt.Current->Collider->Transform), false, &l_color);
			}
		}
		_Core::VectorT_free(&l_hits);
	}

	// Mouse raycast
	{
		_ECS::CameraSystem* l_cameraSystem = (_ECS::CameraSystem*)_ECS::SystemContainerV2_getSystem(&l_interface->ECS->SystemContainerV2, &_ECS::CameraSystemKey);
		_Math::Segment l_ray =

		_ECS::Camera_buildWorldSpaceRay(
			_ECS::CameraSystem_getCurrentActiveCamera(l_cameraSystem),
			_MathV2::Vector2<float> { (float)l_interface->Input->InputMouse.ScreenPosition.x, (float)l_interface->Input->InputMouse.ScreenPosition.y }
		);

		_Physics::RaycastHit l_hit;
		if (_Physics::RayCast(l_interface->PhysicsInterface->World, l_ray.Begin, l_ray.End, &l_hit))
		{
			_Render::Gizmo_drawPoint(l_interface->RenderInterface->Gizmo, (_Math::Vector3f*) &l_hit.HitPoint);
			_Render::Gizmo_drawBox(l_interface->RenderInterface->Gizmo, l_hit.Collider->Box, 
				(_Math::Matrix4x4f*)_Math::Transform_getLocalToWorldMatrix_ref(l_hit.Collider->Transform), false);
		}

	}
}

int main()
{
	{
		TestIntTest l_test{};
		_GameEngine::GameEngineApplication* l_app = IntegrationTest_begin();
		TestInt_init(l_app, &l_test);
		// bool l_exit = false;
		_GameEngine::app_mainLoop(l_app);
		IntegrationTest_end(l_app);
	}
	return 0;
};