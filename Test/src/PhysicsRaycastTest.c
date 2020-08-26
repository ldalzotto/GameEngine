#include "Lib/IntegrationTest.h"

#include "Lib/EntityCreation.h"
#include "Lib/EntityConfigurations.h"

#include "GameEngineApplication.h"

#include "AppEvent/AppEvent.h"

#include "v2/Math.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/TransformC.h"
#include "v2/_interface/MatrixC.h"

#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/Camera.h"
#include "ECSEngine/Systems/CameraRenderSystem.h"

#include "Physics/World/RayCast.h"
#include "Physics/World/Collider/BoxCollider.h"

#include "Renderer/Gizmo/Gizmo.h"

typedef struct TestIntTest_TYP
{
	TransformComponent_PTR PhysicsRayBegin;
	TransformComponent_PTR PhysicsRayEnd;

	TransformComponent_PTR CubeCross1;
	TransformComponent_PTR CubeCross2;
}TestIntTest, * TestIntTest_PTR;

typedef struct CubeCrossCreationInfo_TYP
{
	Vector3f LocalPosition;
	Quaternion4f LocalRotation;
	Vector3f LocalScale;
	TransformComponent_PTR Parent;
	Vector3f* GuidePlaneNormalAxis;
}CubeCrossCreationInfo, * CubeCrossCreationInfo_PTR;

TransformComponent_PTR TestInt_createCubeCross(GameEngineApplicationInterface* l_gameEngine, CubeCrossCreationInfo* p_cubeCrossCreationInfo)
{
	ECS_Entity_HANDLE l_parentEntity;
	TransformComponent_PTR l_parentEntityTransform;

	ECS_Entity_HANDLE l_upLeftEntity;
	TransformComponent_PTR l_upLeftEntityTransform;

	ECS_Entity_HANDLE l_upRightEntity;
	TransformComponent_PTR l_upRightEntityTransform;

	ECS_Entity_HANDLE l_bottomLeftEntity;
	TransformComponent_PTR l_bottomLeftEntityTransform;

	ECS_Entity_HANDLE l_bottomRightEntity;
	TransformComponent_PTR l_bottomRightEntityTransform;

	{
		CubeCreationInfo l_parentCreationInfo = { 0 };
		l_parentCreationInfo.LocalPosition = p_cubeCrossCreationInfo->LocalPosition;
		l_parentCreationInfo.LocalRotation = p_cubeCrossCreationInfo->LocalRotation;
		l_parentCreationInfo.LocalScale = p_cubeCrossCreationInfo->LocalScale;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_parentEntity, &l_parentEntityTransform);
	}


	{
		CubeCreationInfo l_parentCreationInfo = { 0 };
		l_parentCreationInfo.LocalPosition = (Vector3f){ -1.0f, 1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_parentCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_upLeftEntity, &l_upLeftEntityTransform);

		Transform_AddChild(
			&l_parentEntityTransform->Transform,
			&l_upLeftEntityTransform->Transform
		);
	}

	{
		CubeCreationInfo l_parentCreationInfo = { 0 };
		l_parentCreationInfo.LocalPosition = (Vector3f){ 1.0f, 1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_parentCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_upRightEntity, &l_upRightEntityTransform);

		Transform_AddChild(
			&l_parentEntityTransform->Transform,
			&l_upRightEntityTransform->Transform
		);
	}

	{
		CubeCreationInfo l_parentCreationInfo = { 0 };
		l_parentCreationInfo.LocalPosition = (Vector3f){ -1.0f, -1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_parentCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_bottomLeftEntity, &l_bottomLeftEntityTransform);

		Transform_AddChild(
			&l_parentEntityTransform->Transform,
			&l_bottomLeftEntityTransform->Transform
		);
	}

	{
		CubeCreationInfo l_parentCreationInfo = { 0 };
		l_parentCreationInfo.LocalPosition = (Vector3f){ 1.0f, -1.0f, 0.0f };
		l_parentCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_parentCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
		l_parentCreationInfo.WithMeshBound = true;
		l_parentCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		EntityCreation_createEntity(l_gameEngine, &l_parentCreationInfo, &l_bottomRightEntity, &l_bottomRightEntityTransform);

		Transform_AddChild(
			&l_parentEntityTransform->Transform,
			&l_bottomRightEntityTransform->Transform
		);
	}

	if (p_cubeCrossCreationInfo->Parent)
	{
		Transform_AddChild(
			&p_cubeCrossCreationInfo->Parent->Transform,
			&l_parentEntityTransform->Transform
		);
	}

	if (p_cubeCrossCreationInfo->GuidePlaneNormalAxis)
	{
		/*
		_ECS::TransformRotate* l_transformRotate = _ECS::ComponentT_alloc<_ECS::TransformRotate>();
		(l_transformRotate)->Speed = .5f;
		(l_transformRotate)->Axis = *p_cubeCrossCreationInfo->GuidePlaneNormalAxis;
		_ECS::EntityT_addComponentDeferred(l_parentEntity, l_transformRotate, l_gameEngine->ECS);
		*/
	}

	return l_parentEntityTransform;
};



void TestInt_init(GameEngineApplication* l_app, TestIntTest* p_test)
{
	Vector3f tmp_vec3_0;

	EntityConfiguration_init();

	ECS_Entity_HANDLE l_cameraEntity;
	TransformComponent_PTR l_cameraTransform;
	TransformComponent_PTR l_rayTransform;
	TransformComponent_PTR l_sceneModelsRootTransform;

	// Camera
	{
		{
			l_cameraEntity = ECS_AllocateEntity(&l_app->ECS);
		}

		{
			Camera_PTR l_camera = ECS_Component_Alloc_Camera();
			Camera_init(l_camera, &l_app->Render.RenderInterface);
			ECS_AddComponent_Camera(&l_app->ECS, l_cameraEntity, l_camera);
		}

		{
			l_cameraTransform = ECS_Component_Alloc_TransformComponent();
			TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
			l_transformInitInfo.LocalPosition = (Vector3f){ 9.0f, 9.0f, 9.0f };
			tmp_vec3_0 = (Vector3f){ (M_PI * 0.20f), M_PI + (M_PI * 0.25f), 0.0f };

			Vector3f l_target = (Vector3f){ 0.0f, 0.0f,0.0f };
			Matrix3f l_axis;
			Mat_LookAtRotation_F(&l_transformInitInfo.LocalPosition, &l_target, (const Vector3f_PTR)&Vector3f_UP, &l_axis);
			Quat_FromAxis(&l_axis, &l_transformInitInfo.LocalRotation);
			Quat_FromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);

			l_transformInitInfo.LocalScale = (Vector3f){ 1.0f , 1.0f , 1.0f };
			TransformComponent_init(l_cameraTransform, &l_transformInitInfo);
			ECS_AddComponent_TransformComponent(&l_app->ECS, l_cameraEntity, l_cameraTransform);
		}
	}
	// Ray
	{
		ECS_Entity_HANDLE l_rayEntity;
		{
			l_rayEntity = ECS_AllocateEntity(&l_app->ECS);
		}

		{
			TransformComponent_PTR l_transformComponent = ECS_Component_Alloc_TransformComponent();
			l_rayTransform = l_transformComponent;
			TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
			l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, -0.0f, -0.0f };
			tmp_vec3_0 = (Vector3f){ 0.0f, 0.0f, 0.0f };
			Quat_FromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = (Vector3f){ 1.0f , 1.0f , 1.0f };
			TransformComponent_init(l_transformComponent, &l_transformInitInfo);
			ECS_AddComponent_TransformComponent(&l_app->ECS, l_rayEntity, l_transformComponent);
		}

		ECS_Entity_HANDLE l_rayBeginEntity;
		ECS_Entity_HANDLE l_rayEndEntity;

		//Ray Begin
		{
			{
				l_rayBeginEntity = ECS_AllocateEntity(&l_app->ECS);
			}

			{
				TransformComponent_PTR l_transformComponent = ECS_Component_Alloc_TransformComponent();
				p_test->PhysicsRayBegin = l_transformComponent;

				TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
				l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, -0.0f, -100.0f };
				tmp_vec3_0 = (Vector3f){ 0.0f, 0.0f, 0.0f };
				Quat_FromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);
				l_transformInitInfo.LocalScale = (Vector3f){ 1.0f , 1.0f , 1.0f };
				TransformComponent_init(l_transformComponent, &l_transformInitInfo);

				ECS_AddComponent_TransformComponent(&l_app->ECS, l_rayBeginEntity, l_transformComponent);

			}
		}

		//Ray End
		{
			{
				l_rayEndEntity = ECS_AllocateEntity(&l_app->ECS);
			}

			{
				TransformComponent_PTR l_transformComponent = ECS_Component_Alloc_TransformComponent();
				p_test->PhysicsRayEnd = l_transformComponent;

				TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
				l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, -0.0f, 100.0f };
				tmp_vec3_0 = (Vector3f){ 0.0f, 0.0f, 0.0f };
				Quat_FromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);
				l_transformInitInfo.LocalScale = (Vector3f){ 1.0f , 1.0f , 1.0f };
				TransformComponent_init(l_transformComponent, &l_transformInitInfo);

				ECS_AddComponent_TransformComponent(&l_app->ECS, l_rayEndEntity, l_transformComponent);
			}
		}

		Transform_AddChild(&(l_rayTransform)->Transform, &(p_test->PhysicsRayBegin)->Transform);
		Transform_AddChild(&(l_rayTransform)->Transform, &(p_test->PhysicsRayEnd)->Transform);
	}

	// Scene root
	{
		ECS_Entity_HANDLE l_sceneModelsRootEntity;
		{
			l_sceneModelsRootEntity = ECS_AllocateEntity(&l_app->ECS);
		}

		{
			TransformComponent_PTR l_transformComponent = ECS_Component_Alloc_TransformComponent();
			l_sceneModelsRootTransform = l_transformComponent;

			TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
			l_transformInitInfo.LocalPosition = (Vector3f){ 0.0f, 0.0f, 0.0f };
			tmp_vec3_0 = (Vector3f){ 0.0f, 0.0f, 0.0f };
			Quat_FromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = (Vector3f){ 1.0f , 1.0f , 1.0f };
			TransformComponent_init(l_transformComponent, &l_transformInitInfo);

			ECS_AddComponent_TransformComponent(&l_app->ECS, l_sceneModelsRootEntity, l_transformComponent);
		}
	}
	Transform_AddChild(
		&(l_sceneModelsRootTransform)->Transform,
		&(l_rayTransform)->Transform
	);


	// Cubes
	{
		{
			Vector3f l_rotation = { 1.0f, 1.0f, 1.0f };
			CubeCrossCreationInfo l_cubeCrossCreationInfo = { 0 };
			l_cubeCrossCreationInfo.Parent = l_sceneModelsRootTransform;
			l_cubeCrossCreationInfo.LocalPosition = (Vector3f){ 1.0f, 0.0f, 0.0f };
			l_cubeCrossCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
			l_cubeCrossCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
			l_cubeCrossCreationInfo.GuidePlaneNormalAxis = &l_rotation;
			p_test->CubeCross1 = TestInt_createCubeCross(&l_app->GameEngineApplicationInterface, &l_cubeCrossCreationInfo);
		}

		{
			Vector3f l_rotation = { 1.0f, 1.0f, 0.0f };
			CubeCrossCreationInfo l_cubeCrossCreationInfo = { 0 };
			l_cubeCrossCreationInfo.Parent = l_sceneModelsRootTransform;
			l_cubeCrossCreationInfo.LocalPosition = (Vector3f){ 0.0f, -1.0f, 2.0f };
			tmp_vec3_0 = (Vector3f){ 0.0f, M_PI * 0.5f, 0.0f };
			Quat_FromEulerAngle(&tmp_vec3_0, &l_cubeCrossCreationInfo.LocalRotation);
			l_cubeCrossCreationInfo.LocalScale = (Vector3f){ 2.0f, 1.0f, 1.0f };
			l_cubeCrossCreationInfo.GuidePlaneNormalAxis = &l_rotation;
			p_test->CubeCross2 = TestInt_createCubeCross(&l_app->GameEngineApplicationInterface, &l_cubeCrossCreationInfo);
		}
	}



	Vector3f l_forward_test;
	Transform_GetForward(&l_cameraTransform->Transform, &l_forward_test);

	int zd = 0;
	// _Core::CallbackT<TestIntTest, _GameEngine::GameEngineApplicationInterface> l_testUpdate = { TestInt_udpate, p_test };
	// _Core::ObserverT_register(&l_app->EndOfUpdate, (_Core::CallbackT<void, _GameEngine::GameEngineApplicationInterface>*)&l_testUpdate);

}

void TestInt_udpate(GameEngineApplicationInterface* l_interface, TestIntTest* p_test)
{
#ifndef comment
	{
		Vector3f l_rootCenter = { 0.0f, 0.0f, 0.0f };
		Gizmo_DrawTransform_Axis(l_interface->RenderInterface->GizmoBuffer, &l_rootCenter, (const Vector3f_PTR) &Vector3f_RIGHT, (const Vector3f_PTR)&Vector3f_UP, (const Vector3f_PTR)&Vector3f_FORWARD);
	}
#endif

	{
		Vector3f l_axis = { 1.0f, 1.0f, 1.0f };
		Quaternion4f tmp_quat4_0, tmp_quat4_1;
		Quat_RotateAround(&l_axis, l_interface->Clock->DeltaTime * 0.5f, &tmp_quat4_0);
		Quat_Mul(&p_test->CubeCross1->Transform.LocalRotation, &tmp_quat4_0, &tmp_quat4_1);
		Transform_SetLocalRotation(&p_test->CubeCross1->Transform, &tmp_quat4_1);

		l_axis = (Vector3f){ 1.0f, 0.0f, 1.0f };
		Quat_RotateAround(&l_axis, l_interface->Clock->DeltaTime * 0.5f, &tmp_quat4_0);
		Quat_Mul(&p_test->CubeCross2->Transform.LocalRotation, &tmp_quat4_0, &tmp_quat4_1);
		Transform_SetLocalRotation(&p_test->CubeCross2->Transform, &tmp_quat4_1);
	}



	Matrix4f tmp_mat_0;
	{
		Vector3f l_rayBeginPoint; Transform_GetWorldPosition(&(p_test->PhysicsRayBegin)->Transform, &l_rayBeginPoint);
		Vector3f l_rayEndPoint; Transform_GetWorldPosition(&(p_test->PhysicsRayEnd)->Transform, &l_rayEndPoint);
		Vector3c l_color = { 0, 255, 0 };

		Gizmo_DrawLine(l_interface->RenderInterface->GizmoBuffer, &l_rayBeginPoint, &l_rayEndPoint);

		Array_RaycastHit l_hits;
		Arr_Alloc_RaycastHit(&l_hits, 0);
		{
			RayCastAll_Realloc(l_interface->PhysicsInterface->World, &l_rayBeginPoint, &l_rayEndPoint, &l_hits);
			for (size_t i = 0; i < l_hits.Size; i++)
			{
				Gizmo_DrawPoint_Color(l_interface->RenderInterface->GizmoBuffer, &l_hits.Memory[i].HitPoint, &l_color);
				Transform_GetLocalToWorldMatrix(l_hits.Memory[i].Collider->Transform, &tmp_mat_0);
				Gizmo_DrawBox(l_interface->RenderInterface->GizmoBuffer, l_hits.Memory[i].Collider->Box, &tmp_mat_0, false, &l_color);
			}
		}
		Arr_Free(&l_hits.array);
	}

	// Mouse raycast
	{
		CameraRenderSystem_PTR l_cameraSystem = l_interface->GameEngineApplicationSystems.CameraRenderSystem;
		Camera_PTR l_activeCamera = CameraSystem_getCurrentActiveCamera(l_cameraSystem);
		Vector2f l_screenPoint = (Vector2f){ (float)l_interface->Input->InputMouse.ScreenPosition.x, (float)l_interface->Input->InputMouse.ScreenPosition.y };
		Segment_Vector3f l_ray = Camera_buildWorldSpaceRay(l_activeCamera, &l_screenPoint);

		RaycastHit l_hit;
		if (RayCast(l_interface->PhysicsInterface->World, &l_ray.Begin, &l_ray.End, &l_hit))
		{
			Gizmo_DrawPoint(l_interface->RenderInterface->GizmoBuffer, &l_hit.HitPoint);
			Transform_GetLocalToWorldMatrix(l_hit.Collider->Transform, &tmp_mat_0);
			Vector3c l_whiteColor = { 255, 255, 255 };
			Gizmo_DrawBox(l_interface->RenderInterface->GizmoBuffer, l_hit.Collider->Box, &tmp_mat_0, false, &l_whiteColor);
		}

	}
}

int main()
{
	{
		TestIntTest l_test = { 0 };
		GameEngineApplication* l_app = IntegrationTest_begin();
		l_app->Hooks.UpdateAfter = TestInt_udpate;
		l_app->Hooks.UpdateAfterClosure = &l_test;

		TestInt_init(l_app, &l_test);
		// bool l_exit = false;
		app_mainLoop(l_app);
		IntegrationTest_end(l_app);
	}
	return 0;
};