#pragma once

#include "ECS/System.h"
#include "ECS/EntityFilter.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "Functional/Sequencer/SortedSequencer.hpp"

namespace _GameEngine
{
	struct UpdateSequencer;
	namespace _Render { struct RenderInterface; }
	namespace _ECS { struct Entity; struct TransformComponent; struct Camera; }
	namespace _Math {
		struct Vector2f; struct Segment;
	}
}

namespace _GameEngine::_ECS
{
	extern SystemV2Key CameraSystemKey;

	struct CameraSystemOperation
	{
		Entity* Entity;
		Camera* Camera;
		TransformComponent* TransformComponent;
	};

	struct CameraSystem
	{
		SystemHeader SystemHeader;
		EntityFilter EntityFilter;
		_Core::VectorT<CameraSystemOperation> Operations;
	};

	::_Core::SortedSequencerPriority CameraSystem_getUpdatePriority();
	void CameraSystem_alloc(UpdateSequencer* p_updateSequencer, ECS* p_ecs);
	Camera* CameraSystem_getCurrentActiveCamera(CameraSystem* p_system);
	void CameraSystem_buildWorldSpaceRay(Camera* p_camera, _Math::Vector2f* p_screenPoint, _Math::Segment* out_ray);
}