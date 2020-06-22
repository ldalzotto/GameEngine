#pragma once

namespace _GameEngine::_Physics
{
	struct Physics;
	struct World;
}

namespace _GameEngine::_Physics
{
	struct PhysicsInterface
	{
		World* World;
	};

	void PhysicsInterface_build(PhysicsInterface* p_physicsInterface, Physics* p_physics);
}