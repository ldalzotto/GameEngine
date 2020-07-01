
#include "ECS/Component.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformRotateType;

	struct TransformRotate
	{
		float Speed;
		_Math::Vector3f Axis;
	};
}