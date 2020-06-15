#pragma once

#include "ECS/ECS.h"

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_Render
{
	struct Render;
}

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority MeshDrawSystem_getUpdatePriority();
	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
}