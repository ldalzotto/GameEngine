#pragma once

#include "ECS/ECS.h"
#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_ECS
{
	struct CameraSystem
	{
		ECS* ECS;
		_Render::RenderInterface* RenderInterface;
		_Utils::SortedSequencerOperation Update;		
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	_Utils::SortedSequencerPriority CameraSystem_getUpdatePriority();
	System* CameraSystem_alloc(ECS* p_ecs, _Render::RenderInterface* p_renderInterface);
}