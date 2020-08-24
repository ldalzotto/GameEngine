#pragma once

typedef struct World_TYP World;
typedef struct Physics_TYP Physics;

typedef struct PhysicsInterface_TYP
{
	World* World;
}PhysicsInterface, * PhysicsInterface_PTR;

void PhysicsInterface_build(PhysicsInterface* p_physicsInterface, Physics* p_physics);
