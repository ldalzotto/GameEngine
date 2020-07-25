#include "ECSEventQueue.h"

#include <cstdlib>

#include "Algorithm/Compare/CompareAlgorithmT.hpp"

#include "Log/Log.hpp"

#include "ECS/ECS.h"

namespace _GameEngine::_ECS
{

	bool ECSEventMessage_RemoveEntity_equals(ECSEventMessage** l_left, ECSEventMessage_RemoveEntity* l_comparedMessage,  void*);

	void ECSEventQueue_alloc(ECSEventQueue* p_ecsEventQueue, ECS* p_ecs)
	{
		p_ecsEventQueue->ECS = p_ecs;
		_Core::VectorT_alloc(&p_ecsEventQueue->Messages, 0);
	};
	
	void ECSEventQueue_free(ECSEventQueue* p_ecsEventQueue)
	{
#ifndef NDEBUG
		if (p_ecsEventQueue->Messages.Size > 0)
		{
			MYLOG_PUSH(p_ecsEventQueue->ECS->MyLog, ::_Core::LogLevel::WARN, "Potential mmory leak. When the ECSEventQueue is beeing freed, there was still messages pending. Consider corretcly processing messages before free.");
		}
#endif // !NDEBUG

		_Core::VectorT_free(&p_ecsEventQueue->Messages);
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

	ECSEventMessage* ECSEventMessage_AddComponent_alloc(Entity** l_entity, ComponentHeader* l_allocatedComponent)
	{
		ECSEventMessage_AddComponent* l_message = (ECSEventMessage_AddComponent*)malloc(sizeof(ECSEventMessage_AddComponent));
		l_message->MessageType = ECSEventMessageType::ECS_ADD_COMPONENT;
		l_message->Entity = *l_entity;
		l_message->AllocatedComponent = l_allocatedComponent;
		return (ECSEventMessage*)l_message;
	};

	void ECSEventQueue_pushMessage(ECSEventQueue* p_ecsEventQueue, ECSEventMessage** p_ecsEventQueueMessage)
	{
		bool l_eventPushAllowed = true;
		switch ((*p_ecsEventQueueMessage)->MessageType)
		{
			// Having multiple ECS message of destroying the same Entity can result to undefined behavior. So we present it's insertion.
			case ECSEventMessageType::ECS_REMOVE_ENTITY:
				ECSEventMessage_RemoveEntity* l_message = (ECSEventMessage_RemoveEntity*)(*p_ecsEventQueueMessage);
				if (_Core::CompareT_contains(_Core::VectorT_buildIterator(&p_ecsEventQueue->Messages), _Core::ComparatorT<ECSEventMessage*, ECSEventMessage_RemoveEntity, void>{ECSEventMessage_RemoveEntity_equals, l_message}))
				{
					l_eventPushAllowed = false;
				};
				break;
		}

		if(l_eventPushAllowed)
		{
			_Core::VectorT_pushBack(&p_ecsEventQueue->Messages, p_ecsEventQueueMessage);
		}
		else
		{
			free(*p_ecsEventQueueMessage);
		}
	};
	
	void ECSEventQueue_processMessages(ECSEventQueue* p_ecsEventQueue)
	{
		while (p_ecsEventQueue->Messages.Size > 0)
		{
			size_t l_index = 0;
			ECSEventMessage* l_ecsEventMessage = *_Core::VectorT_at(&p_ecsEventQueue->Messages, l_index);
			switch (l_ecsEventMessage->MessageType)
			{
			case ECSEventMessageType::ECS_ADD_ENTITY:
			{
				ECSEventMessage_addEntity* l_message = (ECSEventMessage_addEntity*)l_ecsEventMessage;
				_Core::VectorT_pushBack(&p_ecsEventQueue->ECS->EntityContainer.Entities, &l_message->AllocatedEntityPointer);
			}
			break;
			case ECSEventMessageType::ECS_REMOVE_ENTITY:
			{
				ECSEventMessage_RemoveEntity* l_message = (ECSEventMessage_RemoveEntity*)l_ecsEventMessage;
				_ECS::EntityContainer_freeEntity(&p_ecsEventQueue->ECS->EntityContainer, &l_message->Entity);
			}
			break;
			case ECSEventMessageType::ECS_ADD_COMPONENT:
			{
				ECSEventMessage_AddComponent* l_addComponent = (ECSEventMessage_AddComponent*)l_ecsEventMessage;
				Entity_addComponent(l_addComponent->Entity, l_addComponent->AllocatedComponent, p_ecsEventQueue->ECS);
			}
			break;
			default:
				break;
			}

			free(l_ecsEventMessage);
			_Core::VectorT_erase(&p_ecsEventQueue->Messages, l_index);
		}
	};


}

namespace _GameEngine::_ECS
{

	bool ECSEventMessage_RemoveEntity_equals(ECSEventMessage** l_left, ECSEventMessage_RemoveEntity* l_comparedMessage, void*)
	{
		if ((*l_left)->MessageType == ECSEventMessageType::ECS_REMOVE_ENTITY)
		{
			ECSEventMessage_RemoveEntity* l_leftRemoveEntity = (ECSEventMessage_RemoveEntity*)(*l_left);
			return l_leftRemoveEntity->Entity == l_comparedMessage->Entity;
		}

		return false;
	};
}