#include "Lib/IntegrationTest.h"

#include "Lib/EntityCreation.h"
#include "Lib/EntityConfigurations.h"


#include "GameEngineApplication.h"
#include "v2/Math.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/TransformC.h"
#include "ECS.h"
#include "ECSEngine/Components/Camera.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/Types_def.h"
#include "Physics/World/RayCast.h"
#include "Physics/World/Collider/BoxCollider.h"


void EntitySelectionTest_Init(GameEngineApplication* l_app)
{
	Vector3f tmp_vec3_0;

	// Camera
	{
		ECS_Entity_HANDLE l_cameraEntity = ECS_AllocateEntity(&l_app->ECS);
		{
			Camera_PTR l_camera = ECS_Component_Alloc_Camera();
			Camera_init(l_camera, &l_app->Render.RenderInterface);
			ECS_AddComponent_Camera(&l_app->ECS, l_cameraEntity, l_camera);
		}


		{
			TransformComponent_PTR l_component = ECS_Component_Alloc_TransformComponent();
			TransformInitInfo l_transformInitInfo = TransformInitInfo_Default;
			l_transformInitInfo.LocalPosition = (Vector3f){ 9.0f, 9.0f, 9.0f };
			tmp_vec3_0 = (Vector3f){ (M_PI * 0.20f), M_PI + (M_PI * 0.25f), 0.0f };
			Quat_FromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = (Vector3f){ 1.0f , 1.0f , 1.0f };
			TransformComponent_init(l_component, &l_transformInitInfo);
			ECS_AddComponent_TransformComponent(&l_app->ECS, l_cameraEntity, l_component);
		}
	}
	TransformComponent_PTR l_instranciatedTransform;
	// Cubes
	{
		CubeCreationInfo l_cubeCreationInfo = { 0 };
		l_cubeCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		l_cubeCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
		l_cubeCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_cubeCreationInfo.WithMeshBound = true;
		ECS_Entity_HANDLE l_instancaitedEntity;
		EntityCreation_createEntity(&l_app->GameEngineApplicationInterface, &l_cubeCreationInfo, &l_instancaitedEntity, &l_instranciatedTransform);
	}
	//Child
	{
		CubeCreationInfo l_cubeCreationInfo = {0};
		l_cubeCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		l_cubeCreationInfo.LocalPosition = (Vector3f){ 0.0f, -3.0f, 2.0f };
		l_cubeCreationInfo.LocalScale = (Vector3f){ 1.0f, 1.0f, 1.0f };
		l_cubeCreationInfo.LocalRotation = Quaternion4f_IDENTITY;
		l_cubeCreationInfo.WithMeshBound = true;
		ECS_Entity_HANDLE l_childInstanciatedEntity; TransformComponent_PTR l_childInstranciatedTransform;
		EntityCreation_createEntity(&l_app->GameEngineApplicationInterface, &l_cubeCreationInfo, &l_childInstanciatedEntity, &l_childInstranciatedTransform);

		Transform_AddChild(&l_instranciatedTransform->Transform, &l_childInstranciatedTransform->Transform);
	}
}

int main(int argc, char* argv[])
{
	{
		GameEngineApplication* l_app = IntegrationTest_begin(argv[0]);
		EntityConfiguration_init();
		EntitySelectionTest_Init(l_app);
		// bool l_exit = false;
		app_mainLoop(l_app);
		IntegrationTest_end(l_app);
	}
	return 0;
}