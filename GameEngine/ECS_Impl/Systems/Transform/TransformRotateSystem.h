
#include "ECS/Entity.h"

#include "Utils/SortedSequencer/SortedSequencer.h"

namespace _GameEngine::_ECS
{
	struct ECS;
	struct SystemV2AllocInfo;
}

namespace _GameEngine::_ECS
{
	inline _Utils::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy();
	void TransformRotateSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
}