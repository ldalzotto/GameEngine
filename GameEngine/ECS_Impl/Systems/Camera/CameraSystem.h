#pragma once

#include "ECS/ECS.h"
#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority CameraSystem_getUpdatePriority();
	void CameraSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
}