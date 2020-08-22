#pragma once

#include "Entity_def.h"
#include "Component_def.h"
#include "DataStructures/ARRAY_def.h"

typedef unsigned char ECSEventMessageType;
#define ECSEventMessageType_ADD_ENTITY 0
#define ECSEventMessageType_REMOVE_ENTITY 1
#define ECSEventMessageType_ADD_COMPONENT 2
#define ECSEventMessageType_REMOVE_COMPONENT 3

typedef struct ECS_EventMessage_TYP
{
	ECSEventMessageType MessageType;
}ECS_EventMessage, * ECS_EventMessage_PTR;


typedef struct Array_ECSEventMessagePTR_TYP
{
	ARRAY_TYPE_DEFINITION(ECS_EventMessage_PTR)
}Array_ECSEventMessagePTR, * Array_ECSEventMessagePTR_PTR;

typedef struct ECS_GlobalEvents_TYP
{
	
	Array_ECSEventMessagePTR PendingEvents;

	ECS_OnComponentDestroyedStaticCallback OnComponentDestroyedCallback;

}ECS_GlobalEvents, * ECS_GlobalEvents_PTR;


typedef struct ECS_EventMessage_AddEntity_TYP
{
	ECS_EventMessage Message;
	ECS_Entity_HANDLE AllocatedEntityPointer;
}ECS_EventMessage_AddEntity, *ECS_EventMessage_AddEntity_HANDLE;

typedef struct ECS_EventMessage_RemoveEntity_TYP
{
	ECS_EventMessage Message;
	ECS_Entity_HANDLE Entity;
}ECS_EventMessage_RemoveEntity, *ECS_EventMessage_RemoveEntity_HANDLE;

typedef struct ECS_EventMessage_AddComponent_TYP
{
	ECS_EventMessage Message;
	ECS_Entity_HANDLE Entity;
	ECS_ComponentHeader_HANDLE AllocatedComponent;
}ECS_EventMessage_AddComponent, *ECS_EventMessage_AddComponent_HANDLE;

typedef struct ECS_EventMessage_RemoveComponent_TYP
{
	ECS_EventMessage Message;
	ECS_ComponentHeader_HANDLE RemovedComponent;
}ECS_EventMessage_RemoveComponent, * ECS_EventMessage_RemoveComponent_HANDLE;

