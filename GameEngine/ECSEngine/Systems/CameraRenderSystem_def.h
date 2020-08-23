#pragma once

#include "Entity_def.h"
#include "EntityFilter_def.h"
#include "ECSEngine/Components/Camera_def.h"
#include "ECSEngine/Components/TransformComponent_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct CameraRenderSystemOperation_TYP
{
	ECS_Entity_HANDLE Entity;
	Camera_PTR Camera;
	TransformComponent_PTR TransformComponent;
}CameraRenderSystemOperation, * CameraRenderSystemOperation_PTR;

typedef struct Array_CameraRenderSystemOperation_TYP
{
	ARRAY_TYPE_DEFINITION(CameraRenderSystemOperation)
}Array_CameraRenderSystemOperation, * Array_CameraRenderSystemOperation_PTR;

typedef struct CameraRenderSystem_TYP
{
	ECS_ComponentHeader Header;
	ECS_EntityFilter EntityFilter;
	Array_CameraRenderSystemOperation Operations;
}CameraRenderSystem, *CameraRenderSystem_PTR;