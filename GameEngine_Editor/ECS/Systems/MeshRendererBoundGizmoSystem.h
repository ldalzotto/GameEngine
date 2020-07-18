#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/EntityFilter.hpp"
#include "ECS/System.h"

namespace _GameEngine
{
	struct UpdateSequencer;

	namespace _Render
	{
		struct RenderInterface;
	}

	namespace _ECS
	{
		struct ECS;
		struct SystemV2AllocInfo;
		struct Entity;
		struct TransformComponent;
		struct MeshRendererBound;
	}
}

using namespace _GameEngine;
namespace _GameEngineEditor
{
	struct MeshRendererBoundGizmoSystemOperation
	{
		_ECS::Entity* Entity;
		_ECS::TransformComponent* TransformComponent;
		_ECS::MeshRendererBound* MeshRendererBound;
	};

	struct MeshRendererBoundGizmoSystem
	{
		_ECS::SystemHeader SystemHeader;
		_Core::VectorT<MeshRendererBoundGizmoSystemOperation> Operations;
		_ECS::EntityFilter EntityFilter;
	};

	void MeshRendererBoundGizmoSystem_alloc(_GameEngine::UpdateSequencer* p_updateSequencer, _ECS::ECS* p_ecs);
}