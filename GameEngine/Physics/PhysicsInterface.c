#include "PhysicsInterface.h"

#include "Physics.h"


void PhysicsInterface_build(PhysicsInterface* p_physicsInterface, Physics* p_physics)
{
	p_physicsInterface->World = &p_physics->World;
};
