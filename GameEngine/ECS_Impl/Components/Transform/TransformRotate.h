
#include "ECS/Component.h"
#include "glm/glm.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformRotateType;

	struct TransformRotate
	{
		float Speed;
		glm::vec3 Axis;
	};
}