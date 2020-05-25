
#include "SandboxApplication.h"
#include "Log/Log.h"
#include "ECS/Entity.h"
#include "ECS/MeshRenderer/MeshRenderer.h"

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
		_ECS::Entity l_testEntity{};

		_ECS::Component l_component = _ECS::Component_Build(_ECS::MeshRendererType, new _ECS::MeshRenderer());
		_ECS::Component* l_instanciatedComponent = _ECS::Entity_addComponent(&l_testEntity, l_component);
		_ECS::MeshRenderer* l_meshRenderer = (_ECS::MeshRenderer*)l_instanciatedComponent->Child;

		_ECS::MeshRendererInitInfo l_meshRendererInitInfo{};
		l_meshRendererInitInfo.Render = App->Render;
		l_meshRendererInitInfo.AssociatedComponent = l_instanciatedComponent;
		_ECS::MeshRenderer_init(l_meshRenderer, &l_meshRendererInitInfo);
		_ECS::EntityContainer_pushEntity(&App->EntityComponent->EntityContainer, &l_testEntity);
	}

	HasAlreadyUpdated = true;
}