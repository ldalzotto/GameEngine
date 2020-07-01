
#include "ECS/Component.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_ECS
{
	struct TransformRotate
	{
		float Speed;
		_Math::Vector3f Axis;
	};

	template<>
	extern inline ComponentType* extractComponentType<TransformRotate>();
}