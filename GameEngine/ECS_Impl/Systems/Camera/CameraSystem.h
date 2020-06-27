#pragma once

#include "ECS/ECS.h"
#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _ECS { struct Camera; }
	namespace _Math {
		struct Vector2f; struct Segment;
	}
}

namespace _GameEngine::_ECS
{
	extern SystemV2Key CameraSystemKey;

	_Utils::SortedSequencerPriority CameraSystem_getUpdatePriority();
	void CameraSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
	Camera* CameraSystem_getCurrentActiveCamera(SystemV2* p_system);
	void CameraSystem_buildWorldSpaceRay(Camera* p_camera, _Math::Vector2f* p_screenPoint, _Math::Segment* out_ray);
}