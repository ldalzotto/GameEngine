#include "ECS.h"

namespace _GameEngine::_ECS
{
	void EntityComponent_build(ECS* p_ecs, ::_Core::Log* p_myLog)
	{
		p_ecs->MyLog = p_myLog;
		ECSEventQueue_alloc(&p_ecs->EventQueue, p_ecs);
		SystemContainer_alloc(&p_ecs->SystemContainer);
		EntityContainer_alloc(&p_ecs->EntityContainer);
	};

	void EntityComponent_free(ECS* p_entityComponent)
	{
		EntityContainer_sendEventToDeleteAllEntities(&(p_entityComponent)->EntityContainer, p_entityComponent);
		ECSEventQueue_processMessages(&p_entityComponent->EventQueue);

		EntityContainer_free(&(p_entityComponent)->EntityContainer, &(p_entityComponent)->ComponentEvents);
		SystemContainer_free(&(p_entityComponent)->SystemContainer);
		ECSEventQueue_free(&(p_entityComponent)->EventQueue);
		ComponentEvents_free(&(p_entityComponent)->ComponentEvents, p_entityComponent);
	};
};