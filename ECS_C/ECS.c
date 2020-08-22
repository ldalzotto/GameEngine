#include "ECS.h"
#include "DataStructures/ARRAY.h"
// #include "Log/Log.h"
#include "Error/ErrorHandler.h"

/****************** Global Events *******************/

#include "ECSGlobalEvents_def.h"

inline void Arr_Alloc_ECSEventMessagePTR(Array_ECSEventMessagePTR_PTR p_array, const size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(ECS_EventMessage_PTR), p_initialCapacity); };
inline void Arr_Free_ECSEventMessagePTR(Array_ECSEventMessagePTR_PTR p_array) { Arr_Free((Array_PTR)p_array); };
inline void Arr_Erase_ECSEventMessagePTR(Array_ECSEventMessagePTR_PTR p_array, size_t p_index) { Arr_Erase((Array_PTR)p_array, sizeof(ECS_EventMessage_PTR), p_index); };
inline void Arr_PushBackRealloc_ECSEventMessagePTR(Array_ECSEventMessagePTR_PTR p_array, ECS_EventMessage_PTR p_item) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(ECS_EventMessage_PTR), (char*)(&p_item)); };

void ECS_GlboalEvents_Alloc(ECS_GlobalEvents_PTR p_globalEvents)
{
	Arr_Alloc_ECSEventMessagePTR(&p_globalEvents->PendingEvents, 0);
};

void ECS_GlobalEvents_Free(ECS_GlobalEvents_PTR p_globalEvents)
{
	Arr_Free_ECSEventMessagePTR(&p_globalEvents->PendingEvents);
};

ECS_EventMessage_AddEntity_HANDLE ECS_AllocAddEntityMessage(const ECS_Entity_HANDLE p_allocatedEntity)
{
	ECS_EventMessage_AddEntity_HANDLE l_addEntityMessage = (ECS_EventMessage_AddEntity_HANDLE)malloc(sizeof(ECS_EventMessage_AddEntity));
	l_addEntityMessage->Message.MessageType = ECSEventMessageType_ADD_ENTITY;
	l_addEntityMessage->AllocatedEntityPointer = p_allocatedEntity;
	return l_addEntityMessage;
};

ECS_EventMessage_RemoveEntity_HANDLE ECS_AllocRemoveEntityMessage(const ECS_Entity_HANDLE p_removedEntity)
{
	ECS_EventMessage_RemoveEntity_HANDLE l_removeEntityMessage = (ECS_EventMessage_RemoveEntity_HANDLE)malloc(sizeof(ECS_EventMessage_RemoveEntity));
	l_removeEntityMessage->Message.MessageType = ECSEventMessageType_REMOVE_ENTITY;
	l_removeEntityMessage->Entity = p_removedEntity;
	return l_removeEntityMessage;
};

ECS_EventMessage_AddComponent_HANDLE ECS_AllocAddComponentMessage(const ECS_Entity_HANDLE p_parentEntity, const ECS_ComponentHeader_HANDLE p_attachedComponent)
{
	ECS_EventMessage_AddComponent_HANDLE l_addComponentMessage = (ECS_EventMessage_AddComponent_HANDLE)malloc(sizeof(ECS_EventMessage_AddComponent));
	l_addComponentMessage->Message.MessageType = ECSEventMessageType_ADD_COMPONENT;
	l_addComponentMessage->Entity = p_parentEntity;
	l_addComponentMessage->AllocatedComponent = p_attachedComponent;
	return l_addComponentMessage;
};

ECS_EventMessage_RemoveComponent_HANDLE ECS_AllocRemoveComponentMessage(const ECS_ComponentHeader_HANDLE p_attachedComponent)
{
	ECS_EventMessage_RemoveComponent_HANDLE l_removeComponentMessage = (ECS_EventMessage_RemoveComponent_HANDLE)malloc(sizeof(ECS_EventMessage_RemoveComponent));
	l_removeComponentMessage->Message.MessageType = ECSEventMessageType_REMOVE_COMPONENT;
	l_removeComponentMessage->RemovedComponent = p_attachedComponent;
	return l_removeComponentMessage;
};


/****************** COMPONENT *******************/

#include "Component_def.h"

inline void Arr_Alloc_ComponentHeaderHandle(Array_ComponentHeaderHandle_PTR p_array, const size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(ECS_ComponentHeader_HANDLE), p_initialCapacity); }
inline void Arr_Free_ComponentHeaderHandle(Array_ComponentHeaderHandle_PTR p_array) { Arr_Free((Array_PTR)p_array); }
inline void Arr_PushBackRealloc_ComponentHeaderHandle(Array_ComponentHeaderHandle_PTR p_array, ECS_ComponentHeader_HANDLE p_componentHeader) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(ECS_ComponentHeader_HANDLE), (char*)&p_componentHeader); };
inline void Arr_Erase_ComponentHeaderHandle(Array_ComponentHeaderHandle_PTR p_array, size_t p_index) { Arr_Erase((Array_PTR)p_array, sizeof(ECS_ComponentHeader_HANDLE), p_index); };
inline char Arr_Find_ComponentTypeEquals_ComponentHeaderHandle(Array_ComponentHeaderHandle_PTR p_array, ECS_ComponentType p_comparedComponentType, size_t* p_index)
{
	for (size_t i = 0; i < p_array->Size; i++)
	{
		if (p_array->Memory[i]->ComponentType == p_comparedComponentType)
		{
			if (p_index)
			{
				*p_index = i;
			}
			return 1;
		}
	}
	return 0;
};
 
void ECS_Component_Free(const ECS_ComponentHeader_HANDLE p_component, const ECS_GlobalEvents_PTR p_globalEvents)
{
	p_globalEvents->OnComponentDestroyedCallback.Function(p_component, p_globalEvents->OnComponentDestroyedCallback.Closure);	
	free(p_component);
};


/****************** ENTITY *******************/

#include "Entity_def.h"

ECS_Entity_HANDLE ECS_Entity_Alloc()
{
	ECS_Entity_HANDLE l_instanciatedEntity = (ECS_Entity_HANDLE)malloc(sizeof(ECS_Entity));
	Arr_Alloc_ComponentHeaderHandle(&l_instanciatedEntity->Components, 2);
	return l_instanciatedEntity;
}

void ECS_Entity_Free(ECS_Entity_HANDLE p_entity)
{
	Arr_Free_ComponentHeaderHandle(&p_entity->Components);
}

char ECS_Entity_AddComponent(ECS_Entity_HANDLE p_entity, ECS_ComponentHeader_HANDLE p_component)
{
	if (p_component->AttachedEntity != NULL)
	{
		return 0;
	}
	else if (Arr_Find_ComponentTypeEquals_ComponentHeaderHandle(&p_entity->Components, p_component->ComponentType, NULL))
	{
		return 0;
	}
	else
	{
		Arr_PushBackRealloc_ComponentHeaderHandle(&p_entity->Components, p_component);
		p_component->AttachedEntity = p_entity;
		return 1;
	}
};

inline void Arr_Alloc_ECSEntityHANDLE(Array_ECSEntityHANDLE_PTR p_array, const size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(ECS_Entity_HANDLE), p_initialCapacity); };
inline void Arr_Free_ECSEntityHANDLE(Array_ECSEntityHANDLE_PTR p_array) { Arr_Free((Array_PTR)p_array); };
inline void Arr_PushBackRealloc_ECSEntityHANDLE(Array_ECSEntityHANDLE_PTR p_array, ECS_Entity_HANDLE p_item) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(ECS_Entity_HANDLE), (char*)(&p_item)); };
inline void Arr_Erase_ECSEntityHANDLE(Array_ECSEntityHANDLE_PTR p_array, size_t p_index) { Arr_Erase((Array_PTR)p_array, sizeof(ECS_Entity_HANDLE), p_index); }

inline char Arr_Find_EqualsEntityHANDLE_ECSEntityHANDLE(Array_ECSEntityHANDLE_PTR p_array, const ECS_Entity_HANDLE p_comparedHandle, size_t* out_index)
{
	for (size_t i = 0; i < p_array->Size; i++)
	{
		ECS_Entity_HANDLE l_handle = p_array->Memory[i];
		if (l_handle == p_comparedHandle)
		{
			*out_index = i;
			return 1;
		}
	}
	return 0;
}

void ECS_EntityContainer_Alloc(ECS_EntityContainer_PTR p_entityContainer)
{
	Arr_Alloc_ECSEntityHANDLE(p_entityContainer, 0);
};

void ECS_EntityContainer_SendEventToDeleteAllEntities(ECS_EntityContainer_PTR p_entityContainer, ECS_GlobalEvents_PTR p_globalEvents)
{
	for (size_t i = 0; i < p_entityContainer->Size; i++)
	{
		ECS_EventMessage_RemoveEntity_HANDLE l_removedEntityMessage = ECS_AllocRemoveEntityMessage(p_entityContainer->Memory[i]);
		Arr_PushBackRealloc_ECSEventMessagePTR(&p_globalEvents->PendingEvents, &l_removedEntityMessage->Message);
	}
};

void ECS_EntityContainer_Free(ECS_EntityContainer_PTR p_entityContainer)
{
	Arr_Free_ECSEntityHANDLE(p_entityContainer);
};

/****************** Global Events Processing *******************/

void ECS_GlobalEvents_ProcessMessages(ECS* p_ecs)
{
	while (p_ecs->GlobalEvents.PendingEvents.Size > 0)
	{
		ECS_EventMessage_PTR l_eventMessage = p_ecs->GlobalEvents.PendingEvents.Memory[0];
		switch (l_eventMessage->MessageType)
		{
		case ECSEventMessageType_ADD_ENTITY:
		{
			ECS_EventMessage_AddEntity_HANDLE l_addEntityMessage = (ECS_EventMessage_AddEntity_HANDLE)l_eventMessage;
			Arr_PushBackRealloc_ECSEntityHANDLE(&p_ecs->EntityContainer, l_addEntityMessage->AllocatedEntityPointer);
		}
		break;
		case ECSEventMessageType_REMOVE_ENTITY:
		{
			ECS_EventMessage_RemoveEntity_HANDLE l_removeEntityMessage = (ECS_EventMessage_RemoveEntity_HANDLE)l_eventMessage;
			size_t l_removeIndex;
			if (Arr_Find_EqualsEntityHANDLE_ECSEntityHANDLE(&p_ecs->EntityContainer, l_removeEntityMessage->Entity, &l_removeIndex))
			{
				ECS_Entity_HANDLE l_removedEntity = p_ecs->EntityContainer.Memory[l_removeIndex];

				for (size_t i = 0; i < l_removedEntity->Components.Size; i++)
				{
					ECS_ComponentHeader_HANDLE l_component = l_removedEntity->Components.Memory[i];
					ECS_Component_Free(l_component, &p_ecs->GlobalEvents);
					Arr_Erase_ComponentHeaderHandle(&l_removedEntity->Components, i);
				} 

				ECS_Entity_Free(l_removedEntity);
				Arr_Erase_ECSEntityHANDLE(&p_ecs->EntityContainer, l_removeIndex);
			}
		}
		break;
		case ECSEventMessageType_ADD_COMPONENT:
		{
			ECS_EventMessage_AddComponent_HANDLE l_addComponentMessage = (ECS_EventMessage_AddComponent_HANDLE)l_eventMessage;
			if (!ECS_Entity_AddComponent(l_addComponentMessage->Entity, l_addComponentMessage->AllocatedComponent))
			{
				if (l_addComponentMessage->AllocatedComponent->AttachedEntity == NULL)
				{
					ECS_Component_Free(l_addComponentMessage->AllocatedComponent, &p_ecs->GlobalEvents);
				}
				else
				{
					ABORT_ERROR("ECSEventMessageType_ADD_COMPONENT : Trying to add a component that already have an Entity owner.");
				}
			}
		}
		break;
		case ECSEventMessageType_REMOVE_COMPONENT:
		{
			ECS_EventMessage_RemoveComponent_HANDLE l_removeComponentMessage = (ECS_EventMessage_RemoveComponent_HANDLE)l_eventMessage;
			if (l_removeComponentMessage->RemovedComponent->AttachedEntity)
			{
				size_t l_index;
				if (Arr_Find_ComponentTypeEquals_ComponentHeaderHandle(&l_removeComponentMessage->RemovedComponent->AttachedEntity->Components, l_removeComponentMessage->RemovedComponent->ComponentType, &l_index))
				{
					ECS_Component_Free(l_removeComponentMessage->RemovedComponent, &p_ecs->GlobalEvents);
					Arr_Erase_ComponentHeaderHandle(&l_removeComponentMessage->RemovedComponent->AttachedEntity->Components, l_index);
				}
			}
		}
		break;
		}

		Arr_Erase_ECSEventMessagePTR(&p_ecs->GlobalEvents.PendingEvents, 0);
	}
}


/****************** ECS *******************/

void ECS_Build(ECS* p_ecs, Log_PTR p_myLog)
{
	p_ecs->Log = p_myLog;
	ECS_GlboalEvents_Alloc(&p_ecs->GlobalEvents);
};

void ECS_Free(ECS* p_entityComponent)
{
	ECS_GlobalEvents_ProcessMessages(p_entityComponent);
	ECS_EntityContainer_SendEventToDeleteAllEntities(&p_entityComponent->EntityContainer, &p_entityComponent->GlobalEvents);
	ECS_GlobalEvents_ProcessMessages(p_entityComponent);

	ECS_GlobalEvents_Free(&p_entityComponent->GlobalEvents);
};

ECS_Entity_HANDLE ECS_AllocateEntity(ECS* p_ecs)
{
	ECS_Entity_HANDLE l_entity = ECS_Entity_Alloc();
	ECS_EventMessage_AddEntity_HANDLE l_addEntityMessage = ECS_AllocAddEntityMessage(l_entity);
	Arr_PushBackRealloc_ECSEventMessagePTR(&p_ecs->GlobalEvents.PendingEvents, &l_addEntityMessage->Message);
	return l_entity;
};

void ECS_FreeEntity(ECS* p_ecs, ECS_Entity_HANDLE p_entity)
{
	ECS_EventMessage_RemoveEntity_HANDLE l_removeEntityMessage = ECS_AllocRemoveEntityMessage(p_entity);
	Arr_PushBackRealloc_ECSEventMessagePTR(&p_ecs->GlobalEvents.PendingEvents, &l_removeEntityMessage->Message);
};

ECS_ComponentHeader_HANDLE ECS_Component_Alloc(const ECS_ComponentType p_type, const size_t p_componentSize)
{
	ECS_ComponentHeader_HANDLE l_component = (ECS_ComponentHeader_HANDLE)calloc(1, p_componentSize);
	l_component->ComponentType = p_type;
	return l_component;
};

void ECS_AddComponent(ECS* p_ecs, ECS_Entity_HANDLE p_entity, ECS_ComponentHeader_HANDLE p_component)
{
	ECS_EventMessage_AddComponent_HANDLE l_addComponentHandle = ECS_AllocAddComponentMessage(p_entity, p_component);
	Arr_PushBackRealloc_ECSEventMessagePTR(&p_ecs->GlobalEvents.PendingEvents, &l_addComponentHandle->Message);
};

void ECS_RegisterGlobalComponentDestroyedEvent(ECS* p_ecs, ECS_OnComponentDestroyedStaticCallback_PTR p_callback)
{
	p_ecs->GlobalEvents.OnComponentDestroyedCallback = *p_callback;
};