#pragma once

#include "ECS/EntityFilter.hpp"
#include "ECS/System.h"
#include "Functional/Sequencer/SortedSequencer.hpp"

extern "C"
{
#include "Objects/RenderedObject.h"
}

namespace _GameEngine
{
	struct UpdateSequencer;

	namespace _ECS
	{
		struct ECS;
		struct Entity;
		struct TransformComponent;
		struct MeshRenderer;
	}
}

namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority MeshDrawSystem_updatePriorityBefore();
	
	struct MeshDrawSystemOperation
	{
		Entity* Entity;
		TransformComponent* TransformComponent;
		MeshRenderer* MeshRenderer;
		RenderedObject_HANDLE RenderedObject;
	};

	struct MeshDrawSystem
	{
		SystemHeader Header;
		EntityFilter EntityFilter;
		_Core::VectorT<MeshDrawSystemOperation> MeshDrawSystemOperations;
	};

	void MeshDrawSystem_alloc(UpdateSequencer* p_updateSequencer, ECS* p_ecs);
}