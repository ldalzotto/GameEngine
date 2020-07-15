#pragma once

#include "ECS/ECS.h"

#include "Functional/Sequencer/SortedSequencer.hpp"

namespace _GameEngine::_Render
{
	struct Render;
}

namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority MeshDrawSystem_updatePriorityBefore();
	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
}