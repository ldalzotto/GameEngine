#pragma once

namespace _GameEngine
{
	namespace _ECS
	{
		struct SystemV2AllocInfo;
	}

	struct UpdateSequencer;
}

namespace _GameEngine::_ECS_Impl
{
	void SystemV2Factory_allocSystemV2(_ECS::SystemV2AllocInfo* p_systemV2AllocInfo, UpdateSequencer* p_updateSequencer);
}