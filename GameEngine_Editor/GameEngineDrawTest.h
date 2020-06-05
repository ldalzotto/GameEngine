#pragma once

#include "imgui.h"
#include "GameEngineApplication.h"


#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"

int l_number = 0;

void GameEngineDrawTest_Test(GameEngineApplication* App)
{
	ImGui::Begin("Entity creation");
	ImGui::InputInt("Number", &l_number);
	if (ImGui::Button("CREATE"))
	{
		for (int i = 0; i < l_number; i++)
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
				l_transformRotate->Speed = 1.0f;
				l_transformRotate->Axis = glm::vec3(0.0f, 0.0f, 1.0f);
				_ECS::Entity_addComponent(l_instanciatedEntity, l_component);
			}
		}

	}
	ImGui::End();
}