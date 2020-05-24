#include "EntityComponent.h"

namespace _GameEngine::_ECS
{
	EntityComponent* EntityComponent_alloc()
	{
		return new EntityComponent();
	};

	void EntityComponent_free(EntityComponent** p_entityComponent)
	{
		EntityContainer_free(&(*p_entityComponent)->EntityContainer);
		delete (*p_entityComponent);
		*p_entityComponent = nullptr;
	};
};