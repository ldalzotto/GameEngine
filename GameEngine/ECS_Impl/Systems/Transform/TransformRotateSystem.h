

#include "ECS/ECS.h"

namespace _GameEngine::_ECS
{
	struct TransformRotateSystem
	{
		ECS* ECS;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	void TransformRotateSystem_init(TransformRotateSystem* p_transformRotateSystem, ECS* p_ecs);
	void TransformRotateSystem_free(TransformRotateSystem* p_transformRotateSystem);
	void TransformRotationSystem_update(TransformRotateSystem* p_transformRotateSystem, float p_delta);
}