
#include "ECS/Entity.h"

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_ECS
{
	struct ECS;
	struct System;
}

namespace _GameEngine::_ECS
{
	struct TransformRotateSystem
	{
		ECS* ECS;
		_Utils::SortedSequencerOperation Update;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	System* TransformRotateSystem_alloc(ECS* p_ecs);
	_Utils::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy();
}