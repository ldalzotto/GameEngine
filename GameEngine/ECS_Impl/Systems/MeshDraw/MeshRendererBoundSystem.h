#pragma once

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Physics
{
	struct PhysicsInterface;
}

namespace _GameEngine::_ECS
{
	struct ECS;
	struct SystemV2AllocInfo;
}


namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority();
	void MeshRendererBoundSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs, _Physics::PhysicsInterface* p_physicsInterface);
}