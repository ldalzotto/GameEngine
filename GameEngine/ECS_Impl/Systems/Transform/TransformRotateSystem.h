
#include "ECS/Entity.h"

extern "C"
{
#include "Functional/Sequencer/SortedSequencer.h"
}

namespace _GameEngine::_ECS
{
	struct ECS;
	struct SystemV2AllocInfo;
}

namespace _GameEngine::_ECS
{
	inline SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy();
	void TransformRotateSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs);
}