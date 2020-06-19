#pragma once

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_ECS
{
	struct ECS;
	struct SystemV2AllocInfo;
}

using namespace _GameEngine;
namespace _GameEngineEditor
{
	void MeshRendererBoundGizmoSystem_init(_ECS::SystemV2AllocInfo* p_systemV2AllocInfo, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface);
}