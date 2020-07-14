#include "ECSEventQueue.h"

#include <cstdlib>

#include "Log/Log.hpp"

#include "ECS/ECS.h"

namespace _GameEngine::_ECS
{

	void ECSEventQueue_alloc(ECSEventQueue* p_ecsEventQueue, ECS* p_ecs)
	{
		p_ecsEventQueue->ECS = p_ecs;
		p_ecsEventQueue->Messages.alloc();
	};
	
	void ECSEventQueue_free(ECSEventQueue* p_ecsEventQueue)
	{
#ifndef NDEBUG
		if (p_ecsEventQueue->Messages.size() > 0)
		{
			MYLOG_PUSH(p_ecsEventQueue->ECS->MyLog, ::_Core::LogLevel::WARN, "Potential mmory leak. When the ECSEventQueue is beeing freed, there was still messages pending. Consider corretcly processing messages before free.");
		}
#endif // !NDEBUG

		p_ecsEventQueue->Messages.free();
	};

	ECSEventMessage* ECSEventMessage_addEntity_alloc(Entity** l_allocatedEntity)
	{
		ECSEventMessage_addEntity* l_message = (ECSEventMessage_addEntity*)malloc(sizeof(ECSEventMessage_addEntity));
		l_message->MessageType = ECSEventMessageType::ECS_ADD_ENTITY;
		l_message->AllocatedEntityPointer = *l_allocatedEntity;
		return (ECSEventMessage*)l_message;
	};

	ECSEventMessage* ECSEventMessage_removeEntity_alloc(Entity** l_allocatedEntity)
	{
		ECSEventMessage_RemoveEntity* l_message = (ECSEventMessage_RemoveEntity*)malloc(sizeof(ECSEventMessage_RemoveEntity));
		l_message->MessageType = ECSEventMessageType::ECS_REMOVE_ENTITY;
		l_message->Entity = *l_allocatedEntity;
		return (ECSEventMessage*)l_message;
	};

	ECSEventMessage* ECSEventMessage_AddComponent_alloc(Entity** l_entity, Component** l_allocatedComponent)
	{
		ECSEventMessage_AddComponent* l_message = (ECSEventMessage_AddComponent*)malloc(sizeof(ECSEventMessage_AddComponent));
		l_message->MessageType = ECSEventMessageType::ECS_ADD_COMPONENT;
		l_message->Entity = *l_entity;
		l_message->AllocatedComponent = *l_allocatedComponent;
		return (ECSEventMessage*)l_message;
	};

	void ECSEventQueue_pushMessage(ECSEventQueue* p_ecsEventQueue, ECSEventMessage** p_ecsEventQueueMessage)
	{
		p_ecsEventQueue->Messages.push_back(p_ecsEventQueueMessage);
	};
	
	void ECSEventQueue_processMessages(ECSEventQueue* p_ecsEventQueue)
	{
		while (p_ecsEventQueue->Messages.size() > 0)
		{
			size_t l_index = 0;
			ECSEventMessage* l_ecsEventMessage = *p_ecsEventQueue->Messages.at(l_index);
			switch (l_ecsEventMessage->MessageType)
			{
			case ECSEventMessageType::ECS_ADD_ENTITY:
			{
				ECSEventMessage_addEntity* l_message = (ECSEventMessage_addEntity*)l_ecsEventMessage;
				p_ecsEventQueue->ECS->EntityContainer.Entities.push_back(&l_message->AllocatedEntityPointer);
			}
			break;
			case ECSEventMessageType::ECS_REMOVE_ENTITY:
			{
				ECSEventMessage_RemoveEntity* l_message = (ECSEventMessage_RemoveEntity*)l_ecsEventMessage;
				_ECS::EntityContainer_freeEntity(&l_message->Entity);
			}
			break;
			case ECSEventMessageType::ECS_ADD_COMPONENT:
			{
				ECSEventMessage_AddComponent* l_addComponent = (ECSEventMessage_AddComponent*)l_ecsEventMessage;
				Entity_addComponent(l_addComponent->Entity, l_addComponent->AllocatedComponent);
			}
			break;
			default:
				break;
			}

			free(l_ecsEventMessage);
			p_ecsEventQueue->Messages.erase(l_index);
		}
	};


}