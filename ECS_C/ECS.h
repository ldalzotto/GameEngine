#pragma once

#include "ECS_def.h"

void ECS_Build(ECS* p_ecs, Log_PTR p_myLog);
void ECS_Free(ECS* p_entityComponent);

ECS_Entity_HANDLE ECS_AllocateEntity(ECS* p_ecs);
void ECS_FreeEntity(ECS* p_ecs, ECS_Entity_HANDLE p_entity);

ECS_ComponentHeader_HANDLE ECS_Component_Alloc(const ECS_ComponentType* p_type, const size_t p_componentSize);
void ECS_AddComponent(ECS* p_ecs, ECS_Entity_HANDLE p_entity, ECS_ComponentHeader_HANDLE p_component);