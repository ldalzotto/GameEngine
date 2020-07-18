#pragma once

#include "ECS/System.h"
#include "Functional/Sequencer/SortedSequencer.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/EntityFilter.hpp"

namespace _GameEngine
{
	struct UpdateSequencer;

	namespace _ECS
	{
		struct ECS;
		struct Entity;
		struct SystemV2AllocInfo;
		struct TransformComponent;
		struct TransformRotate;
	}
}

namespace _GameEngine::_ECS
{
	inline ::_Core::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy();

	struct TransformRotateOperation
	{
		Entity* Entity;
		TransformComponent* TransformComponent;
		TransformRotate* TransformRotate;
	};

	struct TransformRotateSystem
	{
		SystemHeader SystemHeader;
		EntityFilter EntityFilter;
		_Core::VectorT<TransformRotateOperation> TransformRotateOperations;
	};

	void TransformRotateSystemV2_alloc(UpdateSequencer* p_updateSequencer, ECS* p_ecs);
}