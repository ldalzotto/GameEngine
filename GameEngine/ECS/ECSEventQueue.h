#pragma once

#include "DataStructures/Specifications/VectorT.hpp"

namespace _GameEngine::_ECS
{
	struct ECS;
	struct Entity;
	struct ComponentHeader;
}

namespace _GameEngine::_ECS
{
	enum class ECSEventMessageType : short
	{
		ECS_ADD_ENTITY,
		ECS_ADD_COMPONENT,
		ECS_REMOVE_ENTITY
	};

	struct ECSEventMessage
	{
		ECSEventMessageType MessageType;
	};

	struct ECSEventMessage_addEntity
	{
		ECSEventMessageType MessageType;
		Entity* AllocatedEntityPointer;
	};

	ECSEventMessage* ECSEventMessage_addEntity_alloc(Entity** l_allocatedEntity);

	struct ECSEventMessage_RemoveEntity
	{
		ECSEventMessageType MessageType;
		Entity* Entity;
	};

	ECSEventMessage* ECSEventMessage_removeEntity_alloc(Entity** l_allocatedEntity);

	struct ECSEventMessage_AddComponent
	{
		ECSEventMessageType MessageType;
		Entity* Entity;
		ComponentHeader* AllocatedComponent;
	};

	ECSEventMessage* ECSEventMessage_AddComponent_alloc(Entity** l_entity, ComponentHeader* l_allocatedComponent);

	struct ECSEventQueue
	{
		ECS* ECS;
		_Core::VectorT<ECSEventMessage*> Messages;
	};

	void ECSEventQueue_alloc(ECSEventQueue* p_ecsEventQueue, ECS* p_ecs);
	void ECSEventQueue_free(ECSEventQueue* p_ecsEventQueue);

	void ECSEventQueue_pushMessage(ECSEventQueue* p_ecsEventQueue, ECSEventMessage** p_ecsEventQueueMessage);
	void ECSEventQueue_processMessages(ECSEventQueue* p_ecsEventQueue);
}