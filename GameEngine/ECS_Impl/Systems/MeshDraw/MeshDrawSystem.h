#pragma once

#include "ECS/EntityFilter.hpp"
#include "ECS/System.h"
#include "Functional/Sequencer/SortedSequencer.hpp"


namespace _RenderV2
{
	struct RenderedObject;
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
		_RenderV2::RenderedObject* RenderedObject;
	};

	struct MeshDrawSystem
	{
		SystemHeader Header;
		EntityFilter EntityFilter;
		_Core::VectorT<MeshDrawSystemOperation> MeshDrawSystemOperations;
	};

	void MeshDrawSystem_alloc(UpdateSequencer* p_updateSequencer, ECS* p_ecs);
}