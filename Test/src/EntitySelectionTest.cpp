#include "Lib/IntegrationTest.h"

#include "Lib/EntityCreation.h"
#include "Lib/EntityConfigurations.h"

#include "GameEngineApplication.h"

#include "Math/Math.h"
#include "Math/Transform/Transform.h"
#include "Math/Quaternion/QuaternionMath.h"
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

void EntitySelectionTest_Init(_GameEngine::GameEngineApplication* l_app)
{
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
			_Math::Quaternion_fromEulerAngles(_Math::Vector3f{ (M_PI * 0.20f), M_PI + (M_PI * 0.25f), 0.0f }, &l_transformInitInfo.LocalRotation);
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
		l_cubeCreationInfo.WithMeshBound = true;
		_ECS::Entity* l_instancaitedEntity; _ECS::TransformComponent* l_instranciatedTransform;
		EntityCreation_createEntity(&l_app->GameEngineApplicationInterface, &l_cubeCreationInfo, &l_instancaitedEntity, &l_instranciatedTransform);
	}
}


int main()
{
	{
		_GameEngine::GameEngineApplication* l_app = IntegrationTest_begin();
		EntityConfiguration_init();
		EntitySelectionTest_Init(l_app);
		// bool l_exit = false;
		_GameEngine::app_mainLoop(l_app);
		IntegrationTest_end(l_app);
	}
	return 0;
}