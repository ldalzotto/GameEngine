#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Functional/Sequencer/SortedSequencer.hpp"
#include "ECS/System.h"
#include "ECS/EntityFilter.hpp"

namespace _GameEngine
{
	struct UpdateSequencer;
}

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Physics
{
	struct PhysicsInterface;
}

namespace _GameEngine::_ECS
{
	struct ECS;
	struct Entity;
	struct SystemV2AllocInfo;
	struct MeshRenderer;
	struct MeshRendererBound;
}


namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority();

	//TODO -> The mesh renderer Box is always calculated by default.
	//This system (and it's component) is only here to feed the Physics engine with box collider (equals to the mesh renderer bounding box).
	//So we hate to rename this system along with the component to something more Physics related.
	struct MeshRendererBoundSystem
	{
		SystemHeader SystemHeader;
		EntityFilter EntityFilter;
		_Physics::PhysicsInterface* PhysicsInterface;
	//	_Core::VectorT<MeshRendererBoundCalculationOperation> MeshRendererBoundsToCaluclate;
	};

	void MeshRendererBoundSystem_alloc(ECS* p_ecs, _Physics::PhysicsInterface* p_physicsInterface, UpdateSequencer* p_updateSequencer);
}