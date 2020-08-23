#include "ECSEngineGlobalEvents.h"

#include "ECSEngine/Components/Types_def.h"
#include "ECSEngine/Components/Camera.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/MeshRenderer.h"

void ECSEngineGlobal_OnComponentDestroyed(ECS_ComponentHeader_HANDLE p_component, void* p_closureTmp)
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
		TransformComponent_Free((TransformComponent_PTR)p_component, ((ECSEngineGlobal_OnComponentDestroyed_Closure_TMP_PTR)p_closureTmp)->ECS);
	}
	break;
	case MESHRENDERER_COMPONENT_TYPE:
	{
		MeshRenderer_OnComponentDetached((MeshRenderer_PTR)p_component, ((ECSEngineGlobal_OnComponentDestroyed_Closure_TMP_PTR)p_closureTmp)->RenderInterface);
	}
	break;
	default:
		break;
	}
};