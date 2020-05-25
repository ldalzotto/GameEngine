
#include "SandboxApplication.h"
#include "Log/Log.h"
#include "ECS/Entity.h"
#include "ECS/MeshRenderer/MeshRenderer.h"
#include "ECS/Transform/Transform.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace _GameEngine;

void SandboxApplication_update(float p_delta);

int main()
{
	App = app_alloc(SandboxApplication_update);

	try
	{
		app_run(App);
	}
	catch (const std::exception& e)
	{
		_Log::LogInstance->ClientLogger->error(e.what());
		app_free(App);
		return EXIT_FAILURE;
	}
	catch (...)
	{
		_Log::LogInstance->ClientLogger->error("Unexpected Error");
		app_free(App);
		return EXIT_FAILURE;
	}

	app_free(App);
	return EXIT_SUCCESS;
};

bool HasAlreadyUpdated = false;

void SandboxApplication_update(float p_delta)
{
	if (!HasAlreadyUpdated)
	{
		_ECS::Entity* l_testEntity = _ECS::Entity_alloc(&App->EntityComponent->EntityContainer);

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::MeshRendererType, new _ECS::MeshRenderer());
			_ECS::MeshRenderer* l_meshRenderer = (_ECS::MeshRenderer*)l_component->Child;

			_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
			l_meshRendererInitInfo.Render = App->Render;
			l_meshRendererInitInfo.AssociatedComponent = l_component;
			_ECS::MeshRenderer_init(l_meshRenderer, &l_meshRendererInitInfo);

			_ECS::Entity_addComponent(l_testEntity, l_component);
		}

		{
			_ECS::Component* l_component = _ECS::Component_alloc(_ECS::TransformType, new _ECS::Transform());
			_ECS::Transform* l_transform = (_ECS::Transform*)l_component->Child;

			_ECS::TransformInitInfo l_transformInitInfo{};
			l_transformInitInfo.LocalPosition = glm::vec3(0.0f);
			l_transformInitInfo.LocalRotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			l_transformInitInfo.LocalPosition = glm::vec3(1.0f);
			_ECS::Transform_init(l_transform, &l_transformInitInfo);

			_ECS::Entity_addComponent(l_testEntity, l_component);
		}

	}

	HasAlreadyUpdated = true;
}