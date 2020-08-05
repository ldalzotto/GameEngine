#include "Lib/IntegrationTest.h"

#include "Lib/EntityCreation.h"
#include "Lib/EntityConfigurations.h"

#include "GameEngineApplication.h"
#include "AppEvent/AppEvent.hpp"

#include "v2/Math.h"
#include "v2/Quaternion/QuaternionMath.hpp"
#include "v2/Transform/TransformM.hpp"

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

void EntitySelectionTest_Init(_GameEngine::GameEngineApplication* l_app)
{
	_MathV2::Vector3<float> tmp_vec3_0;

	// Camera
	{
		_ECS::Entity* l_cameraEntity;
		{
			l_cameraEntity = _ECS::Entity_alloc();
			_ECS::ECSEventMessage* l_addEntityMessage = _ECS::ECSEventMessage_addEntity_alloc(&l_cameraEntity);
			_ECS::ECSEventQueue_pushMessage(&l_app->ECS.EventQueue, &l_addEntityMessage);
		}

		{
			_ECS::Camera* l_camera = _ECS::ComponentT_alloc<_ECS::Camera>();
			_ECS::Camera_init(l_camera, &l_app->Render.RenderInterface);
			_ECS::EntityT_addComponentDeferred(l_cameraEntity, l_camera, &l_app->ECS);
		}

		{
			_ECS::TransformComponent* l_component = _ECS::ComponentT_alloc<_ECS::TransformComponent>();
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = { 9.0f, 9.0f, 9.0f };
			tmp_vec3_0 = { (M_PI * 0.20f), M_PI + (M_PI * 0.25f), 0.0f };
			_MathV2::QuaternionM::fromEulerAngle(&tmp_vec3_0, &l_transformInitInfo.LocalRotation);
			l_transformInitInfo.LocalScale = { 1.0f , 1.0f , 1.0f };
			_ECS::TransformComponent_init(l_component, &l_transformInitInfo);
			_ECS::EntityT_addComponentDeferred(l_cameraEntity, l_component, &l_app->ECS);
		}
	}
	// Cubes
	{
		CubeCreationInfo l_cubeCreationInfo{};
		l_cubeCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
		l_cubeCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
		l_cubeCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
		l_cubeCreationInfo.WithMeshBound = true;
		_ECS::Entity* l_instancaitedEntity; _ECS::TransformComponent* l_instranciatedTransform;
		EntityCreation_createEntity(&l_app->GameEngineApplicationInterface, &l_cubeCreationInfo, &l_instancaitedEntity, &l_instranciatedTransform);

		//Child
		{
			CubeCreationInfo l_cubeCreationInfo{};
			l_cubeCreationInfo.MeshRendererInitInfo = &CubeMeshRendererInit;
			l_cubeCreationInfo.LocalPosition = { 0.0f, -3.0f, 2.0f };
			l_cubeCreationInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
			l_cubeCreationInfo.LocalRotation = _MathV2::Quaternionf_Identity;
			// _Math::Quaternion_fromEulerAngles(_Math::Vector3f{ 1518.0f, 184.0f, -471.0f }, &l_cubeCreationInfo.LocalRotation);
			l_cubeCreationInfo.WithMeshBound = true;
			_ECS::Entity* l_childInstanciatedEntity; _ECS::TransformComponent* l_childInstranciatedTransform;
			EntityCreation_createEntity(&l_app->GameEngineApplicationInterface, &l_cubeCreationInfo, &l_childInstanciatedEntity, &l_childInstranciatedTransform);

			_MathV2::TransformM::addChild(&l_instranciatedTransform->Transform, &l_childInstranciatedTransform->Transform);
		}
	}
}


#ifdef _WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int main()
#endif
{
	{
		_Core::AppEventParams l_params;
#ifdef _WIN32
		l_params.hInstance = hInstance; l_params.hPrevInstance = hPrevInstance; l_params.pCmdLine = pCmdLine; l_params.nCmdShow = nCmdShow;
#endif
		_GameEngine::GameEngineApplication* l_app = IntegrationTest_begin(&l_params);
		EntityConfiguration_init();
		EntitySelectionTest_Init(l_app);
		// bool l_exit = false;
		_GameEngine::app_mainLoop(l_app);
		IntegrationTest_end(l_app);
	}
	return 0;
}