#pragma once

#include "Component_def.h"

typedef struct ECS_TYP ECS;
typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef struct ECSEngineGlobal_OnComponentDestroyed_Closure_TMP_TYP
{
	ECS* ECS;
	RenderV2Interface* RenderInterface;
}ECSEngineGlobal_OnComponentDestroyed_Closure_TMP, * ECSEngineGlobal_OnComponentDestroyed_Closure_TMP_PTR;

void ECSEngineGlobal_OnComponentDestroyed(ECS_ComponentHeader_HANDLE p_component, void*);