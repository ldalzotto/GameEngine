#include "ECS.h"

namespace _GameEngine::_ECS
{
	ECS* EntityComponent_alloc(UpdateSequencer* p_updateSequencer)
	{
		ECS* l_ecs = new ECS();
		l_ecs->UpdateSequencer = p_updateSequencer;
		SystemContainer_alloc(&l_ecs->SystemContainer);
		EntityContainer_alloc(&l_ecs->EntityContainer);
		return l_ecs;
	};

	void EntityComponent_free(ECS** p_entityComponent)
	{
		EntityContainer_free(&(*p_entityComponent)->EntityContainer, &(*p_entityComponent)->ComponentEvents);
		SystemContainer_free(&(*p_entityComponent)->SystemContainer);
		delete (*p_entityComponent);
		*p_entityComponent = nullptr;
	};
};