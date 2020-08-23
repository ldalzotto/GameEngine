#include "ECSEngineGlobalEvents.h"

#include "ECSEngine/Components/Types_def.h"
#include "ECSEngine/Components/Camera.h"
#include "ECSEngine/Components/TransformComponent.h"

void ECSEngineGlobal_OnComponentDestroyed(ECS_ComponentHeader_HANDLE p_component, void* p_ecs)
{
	switch (p_component->ComponentType)
	{
	case CAMERA_TYPE:
	{
		Camera_OnComponentDetached((Camera_PTR)p_component);
	}
	break;
	case TRANSFORM_COMPONENT_TYPE:
	{
		TransformComponent_Free((TransformComponent_PTR)p_component, (ECS*)p_ecs);
	}
	break;
	default:
		break;
	}
};