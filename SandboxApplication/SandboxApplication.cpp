
#include "SandboxApplication.h"
#include "Log/Log.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Transform/TransformRotateSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"

#include "Render/Includes/GLFWIncludes.h"
#include "Render/Texture/Texture.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace _GameEngine;

void SandboxApplication_update(float p_delta);

int main()
{
	App = app_alloc(SandboxApplication_update);
	App_Editor = _GameEngineEditor::GameEngineEditor_alloc(App);

	try
	{
		app_run(App);
	}
	catch (const std::exception& e)
	{
		_Log::LogInstance->ClientLogger->error(e.what());
		app_free(App);
		_GameEngineEditor::GameEngineEditor_free(&App_Editor, App);
		return EXIT_FAILURE;
	}
	catch (...)
	{
		_Log::LogInstance->ClientLogger->error("Unexpected Error");
		app_free(App);
		_GameEngineEditor::GameEngineEditor_free(&App_Editor, App);
		return EXIT_FAILURE;
	}

	app_free(App);
	_GameEngineEditor::GameEngineEditor_free(&App_Editor, App);
	return EXIT_SUCCESS;
};

bool HasAlreadyUpdated = false;

_ECS::Entity* l_cameraEntity;

std::vector<_ECS::Entity*> Entities;

void SandboxApplication_update(float p_delta)
{
	if (!HasAlreadyUpdated)
	{

		l_cameraEntity = _ECS::EntityContainer_allocEntity(App->ECS);

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::CameraType, sizeof(_ECS::Camera));
			_ECS::Camera* l_camera = (_ECS::Camera*)l_component->Child;
			_ECS::CameraDependencies l_cameraDependencies{};
			l_cameraDependencies.SwapChain = &App->Render->SwapChain;
			_ECS::Camera_init(l_camera, l_component, &l_cameraDependencies);
			_ECS::Entity_addComponent(l_cameraEntity, l_component);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformType, sizeof(_ECS::Transform));
			_ECS::Transform* l_transform = (_ECS::Transform*)l_component->Child;
			_Log::LogInstance->CoreLogger->info((void*)l_transform);
			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = glm::vec3(2.0f, 2.0f, 2.0f);
			l_transformInitInfo.LocalRotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			l_transformInitInfo.LocalScale = glm::vec3(1.0f);
			_ECS::Transform_init(l_transform, &l_transformInitInfo);

			_ECS::Entity_addComponent(l_cameraEntity, l_component);
		}

		int COUNT = 1;

		if (Entities.size() == 0)
		{
			for (int i = 0; i < COUNT; i++)
			{

				_ECS::Entity* l_instanciatedEntity = _ECS::EntityContainer_allocEntity(App->ECS);

				{
					_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererType, sizeof(_ECS::MeshRenderer));
					_ECS::MeshRenderer* l_meshRenderer = (_ECS::MeshRenderer*)l_component->Child;

					_Render::DefaultMaterialV2Instance_InputAssets l_defaultMaterialAsset{};
					l_defaultMaterialAsset.MeshPath = "E:/GameProjects/VulkanTutorial/Assets/Models/VikingRoom.obj";
					l_defaultMaterialAsset.Texturepath = "E:/GameProjects/VulkanTutorial/Assets/Textures/viking_room.png";

					_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
					l_meshRendererInitInfo.DefaultMaterialV2Instance_InputAssets = &l_defaultMaterialAsset;
					l_meshRendererInitInfo.AssociatedComponent = l_component;
					_ECS::MeshRenderer_init(l_meshRenderer, &App->Render->RenderInterface, &l_meshRendererInitInfo);

					_ECS::Entity_addComponent(l_instanciatedEntity, l_component);
				}

				{
					_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformType, sizeof(_ECS::Transform));
					_ECS::Transform* l_transform = (_ECS::Transform*)l_component->Child;

					_ECS::TransformInitInfo l_transformInitInfo{};
					l_transformInitInfo.LocalPosition = glm::vec3(0.0f);
					l_transformInitInfo.LocalRotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
					l_transformInitInfo.LocalScale = glm::vec3(1.0f);
					_ECS::Transform_init(l_transform, &l_transformInitInfo);

					_ECS::Entity_addComponent(l_instanciatedEntity, l_component);
				}

				{
					_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformRotateType, sizeof(_ECS::TransformRotate));
					_ECS::TransformRotate* l_transformRotate = (_ECS::TransformRotate*)l_component->Child;
					l_transformRotate->Speed = 1.0f + i;
					l_transformRotate->Axis = glm::vec3(0.0f, 0.0f, 1.0f);
					_ECS::Entity_addComponent(l_instanciatedEntity, l_component);
				}
			//	Entities.push_back(l_instanciatedEntity);
			}

		}


		_ECS::TransformRotateSystem_alloc(App->ECS);
		_ECS::MeshDrawSystem_alloc(App->ECS);
		_ECS::CameraSystem_alloc(App->ECS, &App->Render->RenderInterface);

		HasAlreadyUpdated = true;
	}

}