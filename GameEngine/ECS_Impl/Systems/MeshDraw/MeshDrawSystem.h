#pragma once

#include "ECS/ECS.h"

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_Render
{
	struct Render;
	struct DefaultMaterialDrawStep;
}

namespace _GameEngine::_ECS
{
	struct  MeshDrawSystem
	{
		ECS* ECS;
		_Utils::SortedSequencerOperation Update;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	System* MeshDrawSystem_alloc(ECS* p_ecs);
	_Utils::SortedSequencerPriority MeshDrawSystem_getUpdatePriority();
}