#include "ECS.h"

namespace _GameEngine::_ECS
{
	ECS* EntityComponent_alloc()
	{
		return new ECS();
	};

	void EntityComponent_free(ECS** p_entityComponent)
	{
		EntityContainer_free(&(*p_entityComponent)->EntityContainer, &(*p_entityComponent)->ComponentEvents);
		delete (*p_entityComponent);
		*p_entityComponent = nullptr;
	};
};