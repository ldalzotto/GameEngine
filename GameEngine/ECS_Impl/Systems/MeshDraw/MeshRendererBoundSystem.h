#pragma once

extern "C"
{
#include "Functional/Sequencer/SortedSequencer.h"
}

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
	SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority();
	void MeshRendererBoundSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs, _Physics::PhysicsInterface* p_physicsInterface);
}