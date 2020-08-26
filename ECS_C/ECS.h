#pragma once

#include "ECS_def.h"

void ECS_Build(ECS* p_ecs, Log_PTR p_myLog);
void ECS_Free(ECS* p_entityComponent);

void ECS_GlobalEvents_ProcessMessages(ECS* p_ecs);

ECS_Entity_HANDLE ECS_AllocateEntity(ECS* p_ecs);
void ECS_FreeEntity(ECS* p_ecs, ECS_Entity_HANDLE p_entity);

ECS_ComponentHeader_HANDLE ECS_Component_Alloc(const ECS_ComponentType p_type, const size_t p_componentSize);
void ECS_AddComponent(ECS* p_ecs, ECS_Entity_HANDLE p_entity, ECS_ComponentHeader_HANDLE p_component);
char ECS_GetComponent(ECS_Entity_HANDLE p_entity, const ECS_ComponentType p_type, ECS_ComponentHeader_HANDLE* out_component);

void ECS_RegisterGlobalComponentDestroyedEvent(ECS* p_ecs, ECS_OnComponentDestroyedStaticCallback_PTR p_callback);

#define ECS_ALLOCCOMPONENT_FUNCTION(MethodPrefix, ComponentTypeConstant, ComponentType, ComponentTypeHandle) inline ComponentTypeHandle ECS_Component_Alloc_##MethodPrefix##() { return (ComponentTypeHandle)ECS_Component_Alloc(ComponentTypeConstant, sizeof(ComponentType)); };
#define ECS_ADDCOMPONENT_FUNCTION(MethodPrefix, ComponentTypeHandle) inline void ECS_AddComponent_##MethodPrefix##(ECS* p_ecs, ECS_Entity_HANDLE p_entity, ComponentTypeHandle p_component) { ECS_AddComponent(p_ecs, p_entity, &p_component->Header); };
#define ECS_GETCOMPONENT_FUNCTION(MethodPrefix, ComponentTypeConstant, ComponentTypeHandle) inline char ECS_GetComponent_##MethodPrefix##(ECS_Entity_HANDLE p_entity, ComponentTypeHandle* out_component) { return ECS_GetComponent(p_entity, ComponentTypeConstant, (ECS_ComponentHeader_HANDLE*) out_component); };