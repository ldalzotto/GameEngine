#pragma once

#include "ECS/ECS.h"

extern "C"
{
#include "Functional/Sequencer/SortedSequencer.h"
}

namespace _GameEngine::_Render
{
	struct Render;
}

namespace _GameEngine::_ECS
{
	SortedSequencerPriority MeshDrawSystem_updatePriorityBefore();
	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
}