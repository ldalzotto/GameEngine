#include "Physics.h"

namespace _GameEngine::_Physics
{
	void Physics_alloc(Physics* p_physics, ::_Core::Log* p_myLog)
	{
		PhysicsInterface_build(&p_physics->PhysicsInterface, p_physics);
		World_alloc(&p_physics->World, p_myLog);
	};
	
	void Physics_free(Physics* p_physics)
	{
		World_free(&p_physics->World);
	};
}